#include "GmcServer.hpp"

GmcServer::GmcServer(GmcConnection* client, uint32_t id) : id(id) {
  if (!client) {
    throw std::runtime_error("Cannot create a server for a nullptr client");
  };

  this->client = client;
};

void GmcServer::rcon(std::string command) {
  GmcServerRcon request(this->id, command);

  this->client->send_request(request);
};

void GmcServer::start() {
  throw std::runtime_error("Not implemented");
};

void GmcServer::stop() {
  throw std::runtime_error("Not implemented");
};

void GmcServer::restart() {
  throw std::runtime_error("Not implemented");
};

void GmcServer::status(struct GmcServerStatus& status) {
  GmcServerGetInfo request(this->id, status);

  this->client->send_request(request);
};
