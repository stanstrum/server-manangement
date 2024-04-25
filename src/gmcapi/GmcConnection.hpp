#ifndef GMC_CONNECTION_HPP
#define GMC_CONNECTION_HPP

#include "Paths.hpp"
#include "GmcServer.hpp"
#include <vector>

class GmcConnection {
private:
  std::string host;
  std::string panel_url;

  std::string* session_id = nullptr;
  std::string* csrf_token = nullptr;

  CURL* curl;

  std::vector<struct GmcServer> servers;

  static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
  static size_t header_callback(void* contents, size_t size, size_t nmemb, void* userp);

public:
  GmcConnection(std::string host);
  ~GmcConnection() {
    if (this->session_id) free(this->session_id);
    if (this->csrf_token) free(this->csrf_token);

    curl_easy_cleanup(this->curl);
  };

  template<class Request> void send_request(Request request);

  void connect(std::string username, std::string password);

  void add_server(struct GmcServer& server);
  struct GmcServer* default_server();
};

#endif
