#ifndef PATHS_HPP
#define PATHS_HPP

#include "GmcApiRequest.hpp"
#include "GmcServer.hpp"
#include <iostream>
#include <sstream>
#include <optional>

class GmcCsrfInitialization: private GmcApiRequest {
private:
  std::string m_path;

public:
  GmcCsrfInitialization(std::string path);

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

  struct GmcServerStatus& m_status;

public:
  GmcServerGetInfo(uint32_t server_id, struct GmcServerStatus& m_status);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string response) final;
};

class GmcServerStart: private GmcApiRequest {
private:
  uint32_t id;
  std::string m_path;
  std::string m_referrer;

public:
  GmcServerStart(uint32_t id);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string response) final;
};

class GmcServerStop: private GmcApiRequest {
private:
  std::string m_path;
  std::string m_referrer;

public:
  GmcServerStop(uint32_t id);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string response) final;
};

class GmcServerRcon: private GmcApiRequest {
private:
  std::string m_path;
  std::string m_referrer;
  std::string m_command;

public:
  GmcServerRcon(uint32_t id, std::string command);

  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
  void consume_response(std::string response) final;
};

#endif
