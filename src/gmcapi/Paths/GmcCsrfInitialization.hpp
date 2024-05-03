#ifndef GMC_CSRF_INITIALIZATION_HPP
#define GMC_CSRF_INITIALIZATION_HPP

#include "../GmcApiRequest.hpp"

class GmcCsrfInitialization: private GmcApiRequest {
private:
  std::string m_path;

public:
  GmcCsrfInitialization(std::string path);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string&& response) final;
};

#endif
