#include "GmcConnection.hpp"

#include <iostream>
#include <regex>

class CurlHeaderWrapper {
public:
  struct curl_slist* headers = NULL;

  ~CurlHeaderWrapper() {
    if (this->headers) {
      curl_slist_free_all(this->headers);
    };
  };

  void append(const char* string) {
    struct curl_slist* temp = curl_slist_append(this->headers, string);

    if (!temp) {
      // curl_slist_free_all(this->headers);

      // return some error code?
      throw std::runtime_error("curl_slist_append failed");
    };

    this->headers = temp;
  };
};

GmcConnection::GmcConnection(std::string host)
  : host(host)
{
  // validate hostname & format `panel_url`
  const static std::regex HOST_REGEX("((?:[a-z0-9\\-]*\\.){1,}[a-z0-9\\-]*)");

  if (!std::regex_match(host.begin(), host.end(), HOST_REGEX)) {
    throw std::invalid_argument("Host is malformed");
  };

  this->panel_url = "https://" + host;

  // initialize CURL handle
  CURL* curl = curl_easy_init();

  if (!curl) {
    throw std::runtime_error("CURL initialization failed");
  };

  this->curl = curl;
};

template<class Request> void GmcConnection::send_request(Request request) {
  static_assert(std::is_base_of_v<GmcApiRequest, Request>);

  std::string read_buffer;

  switch (request.method()) {
    case GmcApiRequest::Method::GET:
      curl_easy_setopt(this->curl, CURLOPT_NOBODY, false);
      curl_easy_setopt(this->curl, CURLOPT_POST, false);

      break;

    case GmcApiRequest::Method::POST:
      if (const char* referrer_path = request.referrer()) {
        GmcCsrfInitialization csrf_init(referrer_path);
        this->send_request(csrf_init);
      };

      curl_easy_setopt(this->curl, CURLOPT_NOBODY, false);
      curl_easy_setopt(this->curl, CURLOPT_POST, true);

      break;

    case GmcApiRequest::Method::HEAD:
      curl_easy_setopt(this->curl, CURLOPT_NOBODY, true);
      curl_easy_setopt(this->curl, CURLOPT_POST, false);

      break;
  };

  std::string url = this->panel_url + request.path();

  std::cout << "url: " << url << std::endl;

  curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &read_buffer);
  curl_easy_setopt(this->curl, CURLOPT_HEADERDATA, this);
  curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, this->write_callback);
  curl_easy_setopt(this->curl, CURLOPT_HEADERFUNCTION, this->header_callback);
  curl_easy_setopt(this->curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
  // curl_easy_setopt(this->curl, CURLOPT_VERBOSE, true);
  // curl_easy_setopt(this->curl, CURLOPT_STDERR, stdout);

  CurlHeaderWrapper chw;

  std::string cookie_string = "";

  if (this->csrf_token) {
    cookie_string.append("csrftoken=" + *this->csrf_token);

    if (request.method() == GmcApiRequest::Method::POST) {
      chw.append(("x-csrftoken: " + *this->csrf_token).c_str());
    };
  };

  if (this->session_id) {
    if (!cookie_string.empty()) {
      cookie_string.append("; ");
    };

    cookie_string.append("sessionid=" + *this->session_id);
  };

  if (!cookie_string.empty()) {
    chw.append(("Cookie: " + cookie_string).c_str());
  };

  chw.append(("Origin: " + this->panel_url).c_str());
  chw.append(("Host: " + this->host).c_str());

  const char* referrer = request.referrer();
  std::string referrer_header = "Referer: " + this->panel_url;
  if (referrer) {
    referrer_header += referrer;
    chw.append(referrer_header.c_str());
  };

  curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, chw.headers);
  request.finalize(this->curl);

  // std::cout << "\n";

  CURLcode res = curl_easy_perform(this->curl);

  if (res) {
    throw std::runtime_error("Non-zero CURL result");
  };

  // if (!read_buffer.empty()) {
  //   std::cout << read_buffer << "\n" << std::endl;
  // };

  request.consume_response(std::move(read_buffer));
};

// https://terminalroot.com/using-curl-with-cpp/
size_t GmcConnection::write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
  ((std::string*)userp)->append((char*)contents, size * nmemb);

  return size * nmemb;
};

size_t GmcConnection::header_callback(void* contents, size_t size, size_t nmemb, void* userp) {
  // std::cout << std::string((char*)contents, size * nmemb);

  const static std::regex DEFAULT_SERVER_REGEX("^location: .+?(\\d+)", std::regex_constants::icase);
  const static std::regex COOKIE_REGEX("^set-cookie: (.+?)=(.+?)\\s*[;$]", std::regex_constants::icase);

  GmcConnection* self = (GmcConnection*)userp;
  std::string line((char*)contents, size * nmemb);

  std::smatch cookie_matches;
  std::smatch location_matches;

  std::regex_search(line, cookie_matches, COOKIE_REGEX);
  std::regex_search(line, location_matches, DEFAULT_SERVER_REGEX);

  if (cookie_matches.size() == 3) {
    std::string name = cookie_matches[1];
    std::string value = cookie_matches[2];

    std::string** cookie_to_write;

    if (name == "csrftoken") {
      cookie_to_write = &self->csrf_token;
    } else if (name == "sessionid") {
      cookie_to_write = &self->session_id;
    };

    if (cookie_to_write) {
      // no need to free; replacing the pointer automatically frees
      // *cookie_to_write

      *cookie_to_write = new std::string(value);
    };
  };

  if (
    location_matches.size() == 2 &&
    self->default_server() == nullptr
  ) {
    uint32_t default_server_id = std::stoi(location_matches[1]);

    GmcServer default_server(self, default_server_id);
    self->add_server(default_server);
  };

  return size * nmemb;
};

void GmcConnection::connect(std::string username, std::string password) {
  GmcAuthentication auth { username, password, true };
  this->send_request(auth);

  GmcDefaultServerFetch default_server {};
  this->send_request(default_server);
};

void GmcConnection::add_server(struct GmcServer& server) {
  this->servers.push_back(server);
};

struct GmcServer* GmcConnection::default_server()  {
  if (this->servers.size() != 1)
    return nullptr;

  return &this->servers[0];
};

template void GmcConnection::send_request(GmcCsrfInitialization request);
template void GmcConnection::send_request(GmcAuthentication request);
template void GmcConnection::send_request(GmcDefaultServerFetch request);
template void GmcConnection::send_request(GmcServerGetInfo request);
template void GmcConnection::send_request(GmcServerStart request);
template void GmcConnection::send_request(GmcServerStop request);
template void GmcConnection::send_request(GmcServerRcon request);
template void GmcConnection::send_request(GmcServerConsoleGet request);
