#include "ServerRestartAnnouncer.hpp"

ServerRestartAnnouncer::ServerRestartAnnouncer(
  GmcServer* server,
  const char* env_name
) : DailyEnvIntervalServerOperation(server, env_name) {};

void ServerRestartAnnouncer::operation() {
  this->m_server->rcon("ulx tsay <clr:purple>[<clr:pink>UN<clr:purple>-<clr:pink>SB<clr:purple>]<clr:white> The server is scheduled to restart in exactly one hour.");
};
