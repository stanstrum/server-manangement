#ifndef SERVER_RESTART_ANNOUNCER_HPP
#define SERVER_RESTART_ANNOUNCER_HPP

#include "../scheduler/IntervaledOperation.hpp"

class ServerRestartAnnouncer: public DailyEnvIntervalServerOperation {
protected:
  void operation() final;

public:
  ServerRestartAnnouncer(GmcServer* server, const char* env_name);
};

#endif
