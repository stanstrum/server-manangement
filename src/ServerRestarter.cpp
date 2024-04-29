#include "ServerRestarter.hpp"

void ServerRestarter::operation() {
  this->server->restart();
};

ServerRestarter::ServerRestarter(
  GmcServer* server,
  uint64_t interval,
  time_t start_timestamp
) : IntervaledOperation(interval, start_timestamp), server(server) {};
