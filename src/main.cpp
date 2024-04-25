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

    GmcCsrfInitialization request {};
    conn.send_request(request);

    GmcAuthentication authentication(username, password, true);
    conn.send_request(authentication);

    GmcDefaultServerFetch fetch {};
    conn.send_request(fetch);

    auto* default_server = conn.default_server();
    if (!default_server) {
      std::cerr << "No default server found" << std::endl;

      curl_global_cleanup();

      return 1;
    };

    // default_server->rcon("status");
    struct GmcServerStatus status;
    default_server->status(status);

    std::cout << "server_ipv4_address: " << status.server_ipv4_address << std::endl;
    std::cout << "network_usage: " << status.network_usage << std::endl;
    std::cout << "max_players: " << status.max_players << std::endl;
    std::cout << "ram_usage: " << status.ram_usage << std::endl;
    std::cout << "serv_name: " << status.serv_name << std::endl;
    std::cout << "workshoperror: " << status.workshoperror << std::endl;
    std::cout << "workshop_progress: " << status.workshop_progress << std::endl;
    std::cout << "fps: " << status.fps << std::endl;
    std::cout << "ent_cnt: " << status.ent_cnt << std::endl;
    std::cout << "workshopstatus: " << status.workshopstatus << std::endl;
    std::cout << "pid: " << status.pid << std::endl;
    std::cout << "active_players: " << status.active_players << std::endl;
    std::cout << "server_status: " << status.server_status << std::endl;
    std::cout << "cpu_usage: " << status.cpu_usage << std::endl;
    std::cout << "server_gameport: " << status.server_gameport << std::endl;

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
