#include "gmcapi/GmcConnection.hpp"
#include "ServerRestarter.hpp"

#include "time_units.hpp"

int main(int argc, char** argv) {
  // Remember to do this as well:
  // ulx tsay <clr:purple>[<clr:pink>UN<clr:purple>-<clr:pink>SB<clr:purple>]<clr:white> The server is scheduled to restart in exactly one hour.

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
    conn.connect(username, password);

    auto* default_server = conn.default_server();
    if (!default_server) {
      std::cerr << "No default server found" << std::endl;

      curl_global_cleanup();

      return 1;
    };

    // time_t start_timestamp = 0;
    // ServerRestarter restarter(default_server, 1 * DAY, start_timestamp);

    // restarter.spin();
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

  curl_global_cleanup();

  return 0;
}
