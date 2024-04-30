#ifndef GMC_DEFAULT_SERVER_FETCH_HPP
#define GMC_DEFAULT_SERVER_FETCH_HPP

#include "../GmcApiRequest.hpp"

class GmcDefaultServerFetch: private GmcApiRequest {
public:
  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;

  void consume_response(std::string response) final;
};

#endif
