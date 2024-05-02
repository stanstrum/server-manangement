#ifndef INTERVAL_START_PARSER_HPP
#define INTERVAL_START_PARSER_HPP

#include <chrono>

std::chrono::system_clock::time_point parse_daily_interval_start_from_env(const char* env_var);

#endif
