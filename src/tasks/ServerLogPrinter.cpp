#include "ServerLogPrinter.hpp"

ServerLogPrinter::ServerLogPrinter(
  GmcServer* server,
  std::chrono::system_clock::duration interval
) :
  IntervaledOperation(interval),
  m_server(server)
{
  // Initialize server log start position so we don't print
  // the server's whole history every time
  std::string server_log;
  this->m_server->console_get(server_log);

  this->log_start = server_log.length();
};

void ServerLogPrinter::operation() {
  std::string server_log;
  this->m_server->console_get(server_log);

  if (this->log_start > server_log.length()) {
    return;
  };

  std::cout << server_log.substr(this->log_start);
  this->log_start = server_log.length();
};
