#include "Scheduler.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include <date/date.h>

IntervalExecutor::IntervalExecutor(std::initializer_list<IntervaledOperation*> ops) : ops(ops) {};

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
  IntervaledOperation* soonest_op = nullptr;

  for (auto* op : this->ops) {
    if (!found) {
      soonest_op = op;
      found = true;

      continue;
    };

    if (soonest_op->next_timestamp() > op->next_timestamp()) {
      soonest_op = op;
    };
  };

  if (!found)
    return;

  std::cout << "now  : " << date::format("%DT%T%z", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())) << std::endl;
  std::cout << "until: " << date::format("%DT%T%z", soonest_op->next_timestamp()) << std::endl;

  std::this_thread::sleep_until(soonest_op->next_timestamp());
  soonest_op->run_and_update();
};
