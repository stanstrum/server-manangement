#ifndef GMC_API_REQUEST_HPP
#define GMC_API_REQUEST_HPP

#include <curl/curl.h>
#include <string>

class GmcApiRequest {
public:
  enum Method {
    GET,
    POST,
    HEAD,
  };

  virtual const char* path() = 0;
  virtual const char* referrer() = 0;
  virtual Method method() = 0;

  virtual void finalize(CURL* curl) = 0;

  virtual void consume_response(std::string response) = 0;
};

#endif
