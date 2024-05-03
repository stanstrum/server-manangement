#include "GmcServerConsoleGet.hpp"

#include <sstream>

GmcServerConsoleGet::GmcServerConsoleGet(
  uint32_t server_id,
  std::string& server_log
) : server_log(server_log) {
  std::ostringstream path_oss;
  std::ostringstream referrer_oss;

  path_oss << "/dashboard/game/servers/" << server_id << "/console/get/";
  referrer_oss << "/dashboard/game/servers/" << server_id << "/console/";

  this->m_path = path_oss.str();
  this->m_referrer = referrer_oss.str();
};

const char* GmcServerConsoleGet::path() {
  return this->m_path.c_str();
};

const char* GmcServerConsoleGet::referrer() {
  return this->m_referrer.c_str();
};

GmcApiRequest::Method GmcServerConsoleGet::method() {
  return GET;
};

void GmcServerConsoleGet::finalize(CURL* curl) {};

void GmcServerConsoleGet::consume_response(std::string&& response) {
  this->server_log = response;
};
