#include "gmcapi/GmcConnection.hpp"

#include "ServerRestarter.hpp"
#include "scheduler/Scheduler.hpp"
#include "IntervalStartParser.hpp"
#include "tasks/ServerLogPrinter.hpp"

#include <date/date.h>
#include <date/tz.h>

#include <chrono>


int main(int argc, char** argv) {
  using namespace date;
  using namespace std::chrono;

  // Remember to do this as well:
  // ulx tsay <clr:purple>[<clr:pink>UN<clr:purple>-<clr:pink>SB<clr:purple>]<clr:white> The server is scheduled to restart in exactly one hour.

  auto restart_time = parse_daily_interval_start_from_env("RESTARTER_DAILY_START_TIME");

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

    auto interval = duration(days(1));

    std::cout << "restart_time_sys: " << restart_time << std::endl;
    std::cout << "interval: " << interval << std::endl;

    ServerRestarter restarter(
      default_server,
      interval,
      clock_cast<system_clock>(restart_time)
    );

    ServerLogPrinter printer(default_server, duration(5s));

    IntervalExecutor executor({ &restarter, &printer });
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
