#ifndef GMC_SERVER_STOP_HPP
#define GMC_SERVER_STOP_HPP

#include "../GmcApiRequest.hpp"

class GmcServerStop: private GmcApiRequest {
private:
  std::string m_path;
  std::string m_referrer;

public:
  GmcServerStop(uint32_t id);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string&& response) final;
};

#endif
