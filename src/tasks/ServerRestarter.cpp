#include "ServerRestarter.hpp"

ServerRestarter::ServerRestarter(
  GmcServer* server,
  const char* env_name
) :
  DailyEnvIntervalServerOperation(server, env_name),
  server(server)
{};

void ServerRestarter::operation() {
  std::cout << "[Restarter] Restarting server" << std::endl;

  this->server->restart();
};
