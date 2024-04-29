#include "IntervaledOperation.hpp"

IntervaledOperation::IntervaledOperation(
  const time_t interval,
  time_t next_timestamp
) :
  interval(interval),
  next_timestamp(next_timestamp)
{};

IntervaledOperation::IntervaledOperation(const time_t interval) : interval(interval) {
  this->next_timestamp = time(0) + interval;
};

void IntervaledOperation::run_and_update() {
  this->operation();
  this->next_timestamp += this->interval;
};

time_t IntervaledOperation::seconds_until() {
  time_t now = time(0);

  if (this->next_timestamp <= now) {
    return 0;
  };

  return this->next_timestamp - now;
};
