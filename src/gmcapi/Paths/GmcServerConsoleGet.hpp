#ifndef GMC_SERVER_CONSOLE_GET_HPP
#define GMC_SERVER_CONSOLE_GET_HPP

#include "../GmcApiRequest.hpp"

class GmcServerConsoleGet: private GmcApiRequest {
private:
  std::string m_path;
  std::string m_referrer;

  std::string& server_log;

public:
  GmcServerConsoleGet(uint32_t server_id, std::string& server_log);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string&& response) final;
};

#endif
