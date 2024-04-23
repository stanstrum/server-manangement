#include "GmcConnection.hpp"

#include <iostream>
#include <regex>

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
      curl_easy_setopt(this->curl, CURLOPT_POSTREDIR, false);

      break;

    case GmcApiRequest::Method::POST:
      curl_easy_setopt(this->curl, CURLOPT_POSTREDIR, true);

      break;
  };

  std::string url = this->panel_url + request.path();

  std::cout << "url: " << url << std::endl;

  curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &read_buffer);
  curl_easy_setopt(this->curl, CURLOPT_HEADERDATA, this);
  curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, this->write_callback);
  curl_easy_setopt(this->curl, CURLOPT_HEADERFUNCTION, this->header_callback);

  struct curl_slist* headers = NULL;
  curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

  std::string cookie_string = "";

  if (this->csrf_token) {
    cookie_string.append("csrftoken=" + *this->csrf_token);

    curl_slist_append(headers, ("X-CSRFToken: " + *this->csrf_token).c_str());
  };

  if (this->session_id) {
    if (!cookie_string.empty()) {
      cookie_string.append("; ");
    };

    cookie_string.append("sessionid=" + *this->session_id);
  };

  if (!cookie_string.empty()) {
    curl_slist_append(headers, ("Cookie: " + cookie_string).c_str());
  };

  CURLcode res = curl_easy_perform(curl);

  if (res) {
    std::cerr << "Non-zero CURL result: " << res << std::endl;

    throw new std::runtime_error("Non-zero CURL result");
  };

  // std::cout << prefix_string(header_buffer, "< ");
  std::cout << "(response, " << read_buffer.size() << " bytes)" << std::endl;
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
  const static std::regex CSRF_REGEX("set-cookie: csrftoken=(.+?)\\s*[;$]", std::regex_constants::icase);

  std::string line((char*)contents, size * nmemb);

  std::smatch matches;
  if (
    std::regex_search(line, matches, CSRF_REGEX) &&
    matches.size() >= 2
  ) {
    ((GmcConnection*)userp)->csrf_token = new std::string(matches[1]);

    std::cout << "Got CSRF token! " << *((GmcConnection*)userp)->csrf_token << std::endl;
  };

  ((std::string*)userp)->append((char*)contents, size * nmemb);

  return size * nmemb;
};

template void GmcConnection::send_request(GmcCsrfInitialization request);
template void GmcConnection::send_request(GmcAuthentication request);
