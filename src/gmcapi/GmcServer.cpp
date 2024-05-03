#include "GmcServer.hpp"

GmcServer::GmcServer(GmcConnection* client, uint32_t id) : id(id) {
  if (!client) {
    throw std::runtime_error("Cannot create a server for a nullptr client");
  };

  this->client = client;
};

void GmcServer::rcon(std::string command) {
  this->client->send_request(GmcServerRcon {
    this->id, command
  });
};

void GmcServer::start() {
  this->client->send_request(GmcServerStart {
    this->id
  });
};

void GmcServer::stop() {
  this->client->send_request(GmcServerStop {
    this->id
  });
};

// "Sometimes my genius ... it's almost frightening."
void GmcServer::restart() {
  this->stop();
  this->start();
};

void GmcServer::status(struct GmcServerStatus& status) {
  this->client->send_request(GmcServerGetInfo {
    this->id, status
  });
};

void GmcServer::console_get(std::string& server_log) {
  this->client->send_request(GmcServerConsoleGet {
    this->id, server_log
  });
};
