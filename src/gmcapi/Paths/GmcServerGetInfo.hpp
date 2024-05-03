#ifndef GMC_SERVER_GET_INFO_HPP
#define GMC_SERVER_GET_INFO_HPP

#include "../GmcApiRequest.hpp"

class GmcServerGetInfo: private GmcApiRequest {
private:
  uint32_t m_server_id;
  std::string m_path;

  struct GmcServerStatus& m_status;

public:
  GmcServerGetInfo(uint32_t server_id, struct GmcServerStatus& m_status);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string&& response) final;
};

#endif
