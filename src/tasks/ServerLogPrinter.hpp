#ifndef SERVER_LOG_PRINTER_HPP
#define SERVER_LOG_PRINTER_HPP

#include "../scheduler/IntervaledOperation.hpp"
#include "../gmcapi/GmcServer.hpp"
#include "../IntervalStartParser.hpp"

class ServerLogPrinter: public IntervaledOperation {
private:
  GmcServer* m_server;
  size_t log_start;

protected:
  void operation() final;

public:
  ServerLogPrinter(GmcServer* server, std::chrono::system_clock::duration interval);
};

#endif
