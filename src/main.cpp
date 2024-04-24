#include <iostream>
#include <string>
#include <cstdlib>

#include "gmcapi/GmcConnection.hpp"

int main(int argc, char** argv) {
  std::string username;
  std::string password;
  std::string host;

  if (const char* username_env = std::getenv("USERNAME")) {
    username = username_env;
  } else {
    std::cerr << "No env USERNAME provided" << std::endl;

    return 1;
  };

  if (const char* password_env = std::getenv("PASSWORD")) {
    password = password_env;
  } else {
    std::cerr << "No env PASSWORD provided" << std::endl;

    return 1;
  };

  if (const char* host_env = std::getenv("HOST")) {
    host = host_env;
  } else {
    std::cerr << "No env HOST provided" << std::endl;

    return 1;
  };

  try {
    GmcConnection conn(host);
    conn.debug();

    GmcCsrfInitialization request {};
    conn.send_request(request);

    std::cout << std::endl;

    GmcAuthentication authentication(username, password, true);
    conn.send_request(authentication);

    std::cout << std::endl;

    GmcDefaultServerFetch fetch {};
    conn.send_request(fetch);

    std::cout << std::endl;

    auto* default_server = conn.default_server();
    if (!default_server) {
      std::cerr << "No default server found" << std::endl;

      curl_global_cleanup();

      return 1;
    };

    // default_server->rcon("status");
    default_server->status();

    std::cout << std::endl;

    curl_global_cleanup();
  } catch (const std::runtime_error& error) {
    std::cerr << "Runtime error: " << error.what() << std::endl;

    curl_global_cleanup();

    return 1;
  } catch (const std::invalid_argument& error) {
    std::cerr << "Invalid argument: " << error.what() << std::endl;

    curl_global_cleanup();

    return 1;
  } catch (...) {
    std::exception_ptr p = std::current_exception();
    std::cerr << "Unknown error: " << (p ? p.__cxa_exception_type()->name() : "null") << std::endl;

    curl_global_cleanup();

    return 1;
  };

  return 0;
}
