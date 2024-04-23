#include "Paths.hpp"

const char* GmcCsrfInitialization::path() { return "/login/"; };
const char* GmcCsrfInitialization::referrer() { return 0; };
GmcApiRequest::Method GmcCsrfInitialization::method() { return GET; };

const char* GmcAuthentication::path() { return "/login/auth/"; };
const char* GmcAuthentication::referrer() { return "/login/"; };
GmcApiRequest::Method GmcAuthentication::method() { return POST; };

GmcAuthentication::GmcAuthentication(
  std::string username,
  std::string password,
  bool rememberme
) :
  username(username),
  password(password),
  rememberme(rememberme)
{}
