#include "gmcapi/GmcConnection.hpp"

#include "tasks/ServerLogPrinter.hpp"
#include "tasks/ServerRestarter.hpp"
#include "tasks/ServerRestartAnnouncer.hpp"
#include "scheduler/Scheduler.hpp"

int main(int argc, char** argv) {
  // Get login info from env
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
    // Connect to & authenticate with control panel
    GmcConnection conn(host);
    conn.connect(username, password);

    // Acquire default server (assuming there is one)
    auto* default_server = conn.default_server();
    if (!default_server) {
      std::cerr << "No default server found" << std::endl;

      curl_global_cleanup();
      return 1;
    };

    // Set up tasks
    ServerRestarter restarter(default_server, "RESTARTER_DAILY_START_TIME");
    ServerRestartAnnouncer announcer(default_server, "RESTART_ANNOUNCER_DAILY_START_TIME");
    ServerLogPrinter printer(default_server, duration(5s));

    // Set up executor
    IntervalExecutor executor({ &restarter, &announcer, &printer });

    // Run indefinitely
    executor.spin();
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
