#ifndef SERVER_RESTARTER_HPP
#define SERVER_RESTARTER_HPP

#include "../scheduler/IntervaledOperation.hpp"
#include "../gmcapi/GmcServer.hpp"

class ServerRestarter: public DailyEnvIntervalServerOperation {
private:
  GmcServer* server;

protected:
  void operation() final;

public:
  ServerRestarter(
    GmcServer* server,
    const char* env_name
  );
};

#endif
