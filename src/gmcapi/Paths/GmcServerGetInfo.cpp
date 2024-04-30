#include "GmcServerGetInfo.hpp"

#include "../GmcServer.hpp"
#include <nlohmann/json.hpp>

GmcServerGetInfo::GmcServerGetInfo(
  uint32_t server_id,
  struct GmcServerStatus& status
) :
  m_server_id(server_id),
  m_status(status)
{
  std::ostringstream oss;

  oss << "/dashboard/game/servers/" << this->m_server_id << "/getinfo/";

  this->m_path = oss.str();
};

const char* GmcServerGetInfo::path() { return this->m_path.c_str(); };
const char* GmcServerGetInfo::referrer() { return "/dashboard/profile/"; };
GmcApiRequest::Method GmcServerGetInfo::method() { return GET; };

void GmcServerGetInfo::finalize(CURL* curl) {};
void GmcServerGetInfo::consume_response(std::string response) {
  using json = nlohmann::json;

  json data = json::parse(response);

  try {
    data["server_ipv4_address"].get_to(this->m_status.server_ipv4_address);
    data["network_usage"].get_to(this->m_status.network_usage);
    data["max_players"].get_to(this->m_status.max_players);
    data["ram_usage"].get_to(this->m_status.ram_usage);
    data["serv_name"].get_to(this->m_status.serv_name);
    data["workshoperror"].get_to(this->m_status.workshoperror);
    data["workshop_progress"].get_to(this->m_status.workshop_progress);
    data["fps"].get_to(this->m_status.fps);
    data["ent_cnt"].get_to(this->m_status.ent_cnt);
    data["workshopstatus"].get_to(this->m_status.workshopstatus);
    data["pid"].get_to(this->m_status.pid);
    data["active_players"].get_to(this->m_status.active_players);
    data["server_status"].get_to(this->m_status.server_status);
    data["cpu_usage"].get_to(this->m_status.cpu_usage);
    data["server_gameport"].get_to(this->m_status.server_gameport);
  } catch (const json::type_error& error) {
    std::cerr << error.what() << std::endl;
  };
};
