#include "IntervaledOperation.hpp"
#include "../IntervalStartParser.hpp"

#include <chrono>
#include <date/date.h>

using namespace std::chrono;
IntervaledOperation::IntervaledOperation(
  const system_clock::duration interval,
  time_point<system_clock> next_timestamp
) :
  m_interval(interval),
  m_next_timestamp(next_timestamp)
{};

IntervaledOperation::IntervaledOperation(const system_clock::duration interval) : m_interval(interval) {
  this->m_next_timestamp = system_clock::now();
};

void IntervaledOperation::run_and_update() {
  this->operation();
  this->m_next_timestamp += this->m_interval;
};

system_clock::time_point IntervaledOperation::next_timestamp() {
  return this->m_next_timestamp;
};

DailyEnvIntervalServerOperation::DailyEnvIntervalServerOperation(
  GmcServer* server,
  const char* env_name
) :
  IntervaledOperation {
    duration(date::days(1)),
    parse_daily_interval_start_from_env(env_name)
  },
  m_server(server)
{};
