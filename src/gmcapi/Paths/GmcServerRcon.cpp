#include "GmcServerRcon.hpp"

#include <iostream>
#include <sstream>

GmcServerRcon::GmcServerRcon(uint32_t id, std::string command) : m_command(command) {
  std::ostringstream path_oss;
  std::ostringstream referrer_oss;

  path_oss << "/dashboard/game/servers/" << id << "/console/send/";
  referrer_oss << "/dashboard/game/servers/" << id << "/console/";

  this->m_path = path_oss.str();
  this->m_referrer = referrer_oss.str();
};

const char* GmcServerRcon::path() {
  return this->m_path.c_str();
};

const char* GmcServerRcon::referrer() {
  return this->m_referrer.c_str();
};

GmcApiRequest::Method GmcServerRcon::method() {
  return POST;
};

void GmcServerRcon::finalize(CURL* curl) {
  const char* command = curl_easy_escape(curl, this->m_command.c_str(), this->m_command.size());

  if (!command) {
    throw std::runtime_error("Failed to escape RCON command code");
  };

  std::ostringstream oss;
  oss << "consolecommand=" << command;

  std::string post_data = oss.str();

  std::cout << "post_data: " << post_data << std::endl;
  curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, post_data.c_str());
};

void GmcServerRcon::consume_response(std::string response) {};
