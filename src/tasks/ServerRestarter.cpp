#include "ServerRestarter.hpp"

void ServerRestarter::operation() {
  this->server->restart();
};

ServerRestarter::ServerRestarter(
  GmcServer* server,
  std::chrono::system_clock::duration interval,
  std::chrono::system_clock::time_point start_timestamp
) :
  IntervaledOperation(interval, start_timestamp),
  server(server)
{};
