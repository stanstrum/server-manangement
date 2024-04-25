#include "Paths.hpp"
#include <sstream>

#include <nlohmann/json.hpp>

GmcCsrfInitialization::GmcCsrfInitialization(std::string path) : m_path(path) {};

const char* GmcCsrfInitialization::path() { return this->m_path.c_str(); };
const char* GmcCsrfInitialization::referrer() { return 0; };
GmcApiRequest::Method GmcCsrfInitialization::method() { return HEAD; };

void GmcCsrfInitialization::finalize(CURL* curl) {};
void GmcCsrfInitialization::consume_response(std::string response) {};

const char* GmcAuthentication::path() { return "/login/auth/"; };
const char* GmcAuthentication::referrer() { return "/login/"; };
GmcApiRequest::Method GmcAuthentication::method() { return POST; };

void GmcAuthentication::finalize(CURL* curl) {
  std::ostringstream oss;

  const char *username = curl_easy_escape(curl, this->username.c_str(), this->username.length());
  const char *password = curl_easy_escape(curl, this->password.c_str(), this->password.length());
  const char *rememberme = this->rememberme ? "true" : "false";

  if (!username || !password) {
    throw std::runtime_error("Form data escape failed");
  };

  oss << "username=" << username << "&password=" << password << "&rememberme=" << rememberme;
  std::string post_data = oss.str();

  curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, post_data.c_str());
};

void GmcAuthentication::consume_response(std::string response) {
  // todo, parse response and do stuff
};

GmcAuthentication::GmcAuthentication(
  std::string username,
  std::string password,
  bool rememberme
) :
  username(username),
  password(password),
  rememberme(rememberme)
{}

const char* GmcDefaultServerFetch::path() { return "/dashboard/game/servers/"; };
const char* GmcDefaultServerFetch::referrer() { return "/dashboard/profile/"; };
GmcApiRequest::Method GmcDefaultServerFetch::method() { return HEAD; };

void GmcDefaultServerFetch::finalize(CURL* curl) {};
void GmcDefaultServerFetch::consume_response(std::string response) {};

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

GmcServerStart::GmcServerStart(uint32_t id) {
  std::ostringstream path_oss;
  std::ostringstream referrer_oss;

  path_oss << "/dashboard/game/servers/" << id << "/start/";
  referrer_oss << "/dashboard/game/servers/" << id << "/";

  this->m_path = path_oss.str();
  this->m_referrer = path_oss.str();
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

void GmcServerStop::finalize(CURL* curl) {};
void GmcServerStop::consume_response(std::string response) {};

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
