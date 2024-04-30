#ifndef GMC_SERVER_RCON_HPP
#define GMC_SERVER_RCON_HPP

#include "../GmcApiRequest.hpp"

class GmcServerRcon: private GmcApiRequest {
private:
  std::string m_path;
  std::string m_referrer;
  std::string m_command;

public:
  GmcServerRcon(uint32_t id, std::string command);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string response) final;
};

#endif
