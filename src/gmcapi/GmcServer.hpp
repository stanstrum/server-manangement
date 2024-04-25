#ifndef GMC_SERVER_HPP
#define GMC_SERVER_HPP

#include "GmcConnection.hpp"
#include "Paths.hpp"

#include <iostream>
#include <stdexcept>

struct GmcServerStatus {
  std::string server_ipv4_address;
  uint32_t network_usage;
  uint32_t max_players;
  uint32_t ram_usage;
  std::string serv_name;
  uint32_t workshoperror;
  uint32_t workshop_progress;
  std::string fps;
  std::string ent_cnt;
  uint32_t workshopstatus;
  uint64_t pid;
  uint32_t active_players;
  uint32_t server_status;
  uint32_t cpu_usage;
  std::string server_gameport;
};

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
  void status(struct GmcServerStatus& status);
};

#endif
