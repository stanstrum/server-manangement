#ifndef SERVER_RESTARTER_HPP
#define SERVER_RESTARTER_HPP

#include "scheduler/IntervaledOperation.hpp"
#include "gmcapi/GmcServer.hpp"

class ServerRestarter: IntervaledOperation {
private:
  GmcServer& server;

protected:
  void operation() final;

public:
  ServerRestarter(
    GmcServer& server,
    uint64_t interval,
    time_t start_timestamp
  );
};

#endif
