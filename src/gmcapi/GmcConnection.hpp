#include "Paths.hpp"

class GmcConnection {
private:
  std::string host;
  std::string panel_url;

  std::string* session_id = nullptr;
  std::string* csrf_token = nullptr;

  CURL* curl;

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

  void debug();
};
