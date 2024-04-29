#include "Scheduler.hpp"

#include <thread>
#include <chrono>

void IntervalExecutor::spin() {
  // This prevents spinning forever doing nothing
  if (this->ops.empty())
    return;

  for (;;) {
    this->wait_and_execute();
  };
};

void IntervalExecutor::wait_and_execute() {
  bool found = false;
  time_t seconds_until_next = 0;
  size_t op_index = 0;

  for (size_t i = 0; i < this->ops.size(); i++) {
    auto* op = &this->ops[i];
    time_t op_seconds_until = op->seconds_until();

    if (!found || seconds_until_next > op_seconds_until) {
      seconds_until_next = op_seconds_until;
      op_index = i;
    };
  };

  if (!found)
    return;

  std::this_thread::sleep_for(
    std::chrono::seconds(seconds_until_next)
  );

  this->ops[op_index].run_and_update();
};
