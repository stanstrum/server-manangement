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

  // Ensure we're at the beginning of the line
  while (
    this->log_start > 0 &&
    server_log.at(this->log_start - 1) != '\n'
  ) {
    this->log_start--;

    std::cout << "Backtrack: " << server_log.at(this->log_start) << std::endl;
  };
};

void ServerLogPrinter::operation() {
  // Get server log
  std::string server_log;
  this->m_server->console_get(server_log);

  // Only print text if there's something new
  if (server_log.length() > this->log_start) {
    // new_text cannot be empty here after the above check
    std::string new_text = server_log.substr(this->log_start);

    // Add server prefix to each line of the new text
    for (size_t pos = 0; (pos = new_text.find('\n')) != std::string::npos; ) {
      std::cout << "[Server] " << new_text.substr(0, pos) << std::endl;

      // pos + 1 includes newline
      new_text.erase(0, pos + 1);
    };

    // If the last line is incomplete, don't print it until next time:
    // This means moving back our log_start
    this->log_start -= new_text.length();
  };

  // Update log_start so that we don't print the same text repeatedly
  this->log_start = server_log.length();
};
