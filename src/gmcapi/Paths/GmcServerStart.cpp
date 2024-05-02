#include "GmcServerStart.hpp"

#include <sstream>

GmcServerStart::GmcServerStart(uint32_t id) {
  std::ostringstream path_oss;
  std::ostringstream referrer_oss;

  path_oss << "/dashboard/game/servers/" << id << "/start/";
  referrer_oss << "/dashboard/game/servers/" << id << "/";

  this->m_path = path_oss.str();
  this->m_referrer = referrer_oss.str();
};

const char* GmcServerStart::path() {
  return this->m_path.c_str();
};

const char* GmcServerStart::referrer() {
  return this->m_referrer.c_str();
};

GmcApiRequest::Method GmcServerStart::method() {
  return POST;
};

void GmcServerStart::finalize(CURL* curl) {};
void GmcServerStart::consume_response(std::string response) {};
