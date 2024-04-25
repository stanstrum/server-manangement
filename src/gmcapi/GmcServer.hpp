#ifndef GMC_SERVER_HPP
#define GMC_SERVER_HPP

#include "GmcConnection.hpp"
#include "Paths.hpp"

#include <iostream>
#include <stdexcept>

struct GmcServerStatus {
  std::string server_ipv4_address; // "208.103.169.107"
  uint8_t network_usage; // 0
  uint32_t max_players; // 32
  uint8_t ram_usage; // 2
  std::string serv_name; // "Universe Sandbox [Need Staff] |Wiremod|B"
  uint8_t workshoperror; // 0
  uint8_t workshop_progress; // 0
  std::string fps; // ""
  std::string ent_cnt; // ""
  uint32_t workshopstatus; // 0
  uint64_t pid; // 28564
  uint32_t active_players; // 0
  uint32_t server_status; // 1
  uint8_t cpu_usage; // 4
  uint16_t server_gameport; // "27016"
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
