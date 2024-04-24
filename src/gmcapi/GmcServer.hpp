#ifndef GMC_SERVER_HPP
#define GMC_SERVER_HPP

#include "GmcConnection.hpp"
#include <stdexcept>

class GmcServer {
private:
  uint32_t id;
  struct GmcConnection* client;

public:
  GmcServer(GmcConnection* client, uint32_t id) : id(id) {
    if (!client) {
      throw new std::runtime_error("Cannot create a server for a nullptr client");
    };

    this->client = client;
  };
};

#endif
