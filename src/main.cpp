#include "gmcapi/GmcConnection.hpp"

#include "ServerRestarter.hpp"
#include "scheduler/Scheduler.hpp"

#include <date/date.h>
#include <date/tz.h>

#include <chrono>

int main(int argc, char** argv) {
  std::cout << "input: " << std::flush;

  std::chrono::system_clock::duration time_of_day;
  std::string timezone;

  date::from_stream(std::cin, "%I:%M%t%p %Z", time_of_day, &timezone);

  if (std::cin.fail()) {
    std::cerr << "Failed to parse server restart time" << std::endl;

    return 1;
  };

  using date::operator<<;
  std::cout << "info: " << date::format("%I:%M %p", time_of_day) << std::endl;
  std::cout << "timezone: " << timezone << std::endl;
  std::cout << date::locate_zone(timezone)->name() << std::endl;

  // Remember to do this as well:
  // ulx tsay <clr:purple>[<clr:pink>UN<clr:purple>-<clr:pink>SB<clr:purple>]<clr:white> The server is scheduled to restart in exactly one hour.

  using namespace date;
  using namespace std::chrono;

  auto target_tz = locate_zone(timezone);

  auto now_utc = floor<seconds>(system_clock::now());
  auto now_est = make_zoned(target_tz, now_utc).get_local_time();

  // Find the beginning of the day, EST
  auto begin_of_day_est = floor<days>(now_est);

  // 4:00am EST
  auto local_restart_time = begin_of_day_est + time_of_day;

  // Convert back to UTC
  auto restart_time = zoned_time(target_tz, local_restart_time).get_sys_time();

  // Ensure the next restart time is in the future
  if (now_utc > restart_time) {
    restart_time += days(1);
  };

  auto difference = restart_time - now_utc;

  std::cout << "now_utc: " << now_utc << std::endl;
  std::cout << "now_est: " << now_est << std::endl;
  std::cout << "begin_of_day_est: " << begin_of_day_est << std::endl;
  std::cout << "restart_time: " << restart_time << std::endl;
  std::cout << "difference: " << difference << std::endl;

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

    auto interval = system_clock::duration(days(1));

    std::cout << "restart_time_sys: " << restart_time << std::endl;
    std::cout << "interval: " << interval << std::endl;

    ServerRestarter restarter(
      default_server,
      interval,
      clock_cast<system_clock>(restart_time)
    );

    IntervalExecutor executor({ &restarter });
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
