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
      throw new std::runtime_error("curl_slist_append failed");
    };

    this->headers = temp;
  };
};

std::string prefix_string(std::string str, std::string prefix) {
  std::string out = prefix;

  for (size_t i = 0; i < str.length(); i++) {
    out.push_back(str[i]);

    if (str[i] == '\n') {
      out.append(prefix);
    };
  };

  return out;
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
    std::cout << "add CSRF to cookie string" << std::endl;

    cookie_string.append("csrftoken=" + *this->csrf_token);

    chw.append(("x-csrftoken: " + *this->csrf_token).c_str());
  };

  if (this->session_id) {
    std::cout << "add session id to cookie string" << std::endl;

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
    std::cout << "added referrer" << std::endl;

    referrer_header += referrer;

    chw.append(referrer_header.c_str());
  } else {
    std::cout << "no referrer" << std::endl;
  };

  curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, chw.headers);

  request.finalize(this->curl);

  // std::cout << "\n\n" << std::endl;
  CURLcode res = curl_easy_perform(this->curl);

  if (res) {
    std::cerr << "Non-zero CURL result: " << res << std::endl;

    throw new std::runtime_error("Non-zero CURL result");
  };

  std::cout << prefix_string(read_buffer, "< ");
  std::cout << "(response, " << read_buffer.size() << " bytes)" << std::endl;

  return request.consume_response(read_buffer);
};

void GmcConnection::debug() {
  std::cout << "host: " << this->host << '\n';
  std::cout << "panel_url: " << this->panel_url << std::endl;
};

// https://terminalroot.com/using-curl-with-cpp/
size_t GmcConnection::write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
  ((std::string*)userp)->append((char*)contents, size * nmemb);

  return size * nmemb;
};

size_t GmcConnection::header_callback(void* contents, size_t size, size_t nmemb, void* userp) {
  const static std::regex CSRF_REGEX("set-cookie: (.+?)=(.+?)\\s*[;$]", std::regex_constants::icase);

  std::string line((char*)contents, size * nmemb);

  std::string name;
  std::string value;
  std::string** cookie_to_write;

  GmcConnection* self;

  std::smatch matches;
  if (
    std::regex_search(line, matches, CSRF_REGEX) &&
    matches.size() < 3 // [whole match, group 1, group 2] =
  ) {
    goto ret;
  };

  name = matches[1];
  value = matches[2];

  self = (GmcConnection*)userp;

  if (name == "csrftoken") {
    std::cout << "Got CSRF token! " << value << std::endl;
    cookie_to_write = &self->csrf_token;
  } else if (name == "sessionid") {
    std::cout << "Got session id! " << value << std::endl;
    cookie_to_write = &self->session_id;
  } else {
    goto ret;
  };

  if (*cookie_to_write) {
    free(*cookie_to_write);
  };

  *cookie_to_write = new std::string(value);

  ret: return size * nmemb;
};

template void GmcConnection::send_request(GmcCsrfInitialization request);
template void GmcConnection::send_request(GmcAuthentication request);
