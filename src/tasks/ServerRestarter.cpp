#include "ServerRestarter.hpp"

ServerRestarter::ServerRestarter(
  GmcServer* server,
  std::chrono::system_clock::duration interval,
  std::chrono::system_clock::time_point start_timestamp
) :
  IntervaledOperation(interval, start_timestamp),
  server(server)
{};

void ServerRestarter::operation() {
  std::cout << "[Restarter] Restarting server" << std::endl;

  this->server->restart();
};
