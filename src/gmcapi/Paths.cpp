#include "GmcApiRequest.hpp"
#include <string>

class GmcCsrfInitialization: private GmcApiRequest {
  public:
    const char* path() { return "/"; };
    const char* referrer() { return 0; };
    const char* method() { return "GET"; };
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
    ) :
      username(username),
      password(password),
      rememberme(rememberme)
    {}

    const char* path() { return "/login/auth/"; };
    const char* referrer() { return "/login/"; };
    const char* method() { return "POST"; };
};
