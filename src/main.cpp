#include <iostream>
#include <string>

#include "gmcapi/GmcConnection.hpp"

#include <curl/curl.h>

int main(int argc, char** argv) {
  GmcConnection conn("cp-dal04.gmchosting.com");
  conn.debug();

  GmcCsrfInitialization request {};
  conn.send_request(request);

  return 0;
}
