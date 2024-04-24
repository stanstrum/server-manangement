#include "GmcApiRequest.hpp"
#include <string>

class GmcCsrfInitialization: private GmcApiRequest {
public:
  const char* path() final;
  const char* referrer() final;
  Method method() final;

  void finalize(CURL* curl) final;
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
};
