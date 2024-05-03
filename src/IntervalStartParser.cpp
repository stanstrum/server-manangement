#include "IntervalStartParser.hpp"

#include <iostream>
#include <exception>
#include <sstream>

#include <date/date.h>
#include <date/tz.h>

std::chrono::system_clock::time_point parse_daily_interval_start_from_env(const char* env_var) {
  using namespace date;
  using namespace std::chrono;

  // Grab env variable contents
  std::string start_info;

  if (const char* env_content = std::getenv(env_var)) {
    start_info = env_content;
  } else {
    std::cerr << "No env " << env_var << " provided" << std::endl;

    throw std::runtime_error("Failed to parse interval start: env var not present");
  };

  // Parse info from string
  std::istringstream start_info_stream(start_info);

  system_clock::duration time_of_day;
  std::string timezone;

  from_stream(start_info_stream, "%I:%M%t%p %Z", time_of_day, &timezone);

  if (start_info_stream.fail()) {
    std::cerr << "Failed to parse server restart time: env " << env_var << " is invalid" << std::endl;

    throw std::runtime_error("Failed to parse server restart time");
  };

  // Debug
  std::cout << "[" << env_var << "] info: " << format("%I:%M %p", time_of_day) << std::endl;
  std::cout << "[" << env_var << "] timezone: " << timezone << std::endl;

  // Calculate daily start time
  auto target_tz = locate_zone(timezone);

  auto now_utc = floor<seconds>(system_clock::now());
  auto now_tz = make_zoned(target_tz, now_utc).get_local_time();

  // Find the beginning of the day in the appropriate timezone
  auto begin_of_day_est = floor<days>(now_tz);

  // This is the proper start time in the local timezone
  auto local_start_time = begin_of_day_est + time_of_day;

  // Convert back to UTC
  auto start_time = zoned_time(target_tz, local_start_time).get_sys_time();

  // Ensure the next restart time is in the future
  if (now_utc > start_time) {
    start_time += days(1);
  };

  auto difference = floor<seconds>(start_time - system_clock::now());

  std::cout << "[" << env_var << "] now_utc: " << now_utc << std::endl;
  std::cout << "[" << env_var << "] now_tz: " << now_tz << std::endl;
  std::cout << "[" << env_var << "] begin_of_day_est: " << begin_of_day_est << std::endl;
  std::cout << "[" << env_var << "] start_time: " << start_time << std::endl;
  std::cout << "[" << env_var << "] difference: " << difference << std::endl;

  return start_time;
};
