#ifndef GMC_SERVER_START_HPP
#define GMC_SERVER_START_HPP

#include "../GmcApiRequest.hpp"

class GmcServerStart: private GmcApiRequest {
private:
  uint32_t id;
  std::string m_path;
  std::string m_referrer;

public:
  GmcServerStart(uint32_t id);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string&& response) final;
};

#endif
