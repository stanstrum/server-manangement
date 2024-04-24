#ifndef GMC_SERVER_HPP
#define GMC_SERVER_HPP

#include "GmcConnection.hpp"
#include "Paths.hpp"

#include <iostream>
#include <stdexcept>

class GmcServer {
private:
  uint32_t id;
  struct GmcConnection* client;

public:
  GmcServer(GmcConnection* client, uint32_t id);

  void rcon(std::string command);
  void start();
  void stop();
  void restart();
  void status();
};

#endif
