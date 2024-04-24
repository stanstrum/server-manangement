#ifndef PATHS_HPP
#define PATHS_HPP

#include "GmcApiRequest.hpp"
#include "GmcServer.hpp"
#include <iostream>
#include <sstream>
#include <optional>

class GmcCsrfInitialization: private GmcApiRequest {
public:
  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;

  void consume_response(std::string response) final;
};

class GmcAuthentication: private GmcApiRequest {
private:
  std::string username;
  std::string password;
  bool rememberme;

public:
  GmcAuthentication(
    std::string username,
    std::string password,
    bool rememberme
  );

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;

  void consume_response(std::string response) final;
};

class GmcDefaultServerFetch: private GmcApiRequest {
public:
  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;

  void consume_response(std::string response) final;
};

class GmcServerGetInfo: private GmcApiRequest {
private:
  uint32_t m_server_id;
  std::string m_path;

public:
  GmcServerGetInfo(uint32_t server_id);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string response) final;
};

#endif
