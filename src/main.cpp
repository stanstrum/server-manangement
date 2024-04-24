#include <iostream>
#include <string>

#include "gmcapi/GmcConnection.hpp"

int main(int argc, char** argv) {
  GmcConnection conn("cp-dal04.gmchosting.com");
  conn.debug();

  GmcCsrfInitialization request {};
  conn.send_request(request);

  std::cout << std::endl;

  GmcAuthentication authentication("username", "password", true);
  conn.send_request(authentication);

  std::cout << std::endl;

  curl_global_cleanup();

  return 0;
}
