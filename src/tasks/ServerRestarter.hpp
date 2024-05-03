#ifndef SERVER_RESTARTER_HPP
#define SERVER_RESTARTER_HPP

#include "../scheduler/IntervaledOperation.hpp"
#include "../gmcapi/GmcServer.hpp"

class ServerRestarter: public IntervaledOperation {
private:
  GmcServer* server;

protected:
  void operation() final;

public:
  ServerRestarter(
    GmcServer* server,
    std::chrono::system_clock::duration interval,
    std::chrono::system_clock::time_point start_timestamp
  );
};

#endif
