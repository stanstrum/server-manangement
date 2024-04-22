#include "GmcConnection.hpp"

#include <iostream>
#include <regex>

GmcConnection::GmcConnection(std::string host)
  : host(host)
{
  static std::regex HOST_REGEX("((?:[a-z0-9\\-]*\\.){1,}[a-z0-9\\-]*)");

  if (!std::regex_match(host.begin(), host.end(), HOST_REGEX)) {
    throw std::invalid_argument("Host is malformed");
  };

  this->panel_url = "https://" + host;
};

void GmcConnection::debug() {
  std::cout << "host: " << this->host << '\n';
  std::cout << "panel_url: " << this->panel_url << std::endl;
};
