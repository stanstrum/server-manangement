#include "GmcServerStop.hpp"

#include <sstream>

GmcServerStop::GmcServerStop(uint32_t id) {
  std::ostringstream path_oss;
  std::ostringstream referrer_oss;

  path_oss << "/dashboard/game/servers/" << id << "/stop/";
  referrer_oss << "/dashboard/game/servers/" << id << "/";

  this->m_path = path_oss.str();
  this->m_referrer = path_oss.str();
};

const char* GmcServerStop::path() {
  return this->m_path.c_str();
};

const char* GmcServerStop::referrer() {
  return this->m_referrer.c_str();
};

GmcApiRequest::Method GmcServerStop::method() {
  return POST;
};

void GmcServerStop::finalize(CURL* curl) {
  // TODO: add stopcmd
  curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, "stopcmd=");
};
void GmcServerStop::consume_response(std::string response) {};
