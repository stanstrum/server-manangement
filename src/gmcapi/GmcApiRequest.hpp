#include <curl/curl.h>

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
};
