#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "IntervaledOperation.hpp"
#include <vector>

class IntervalExecutor {
private:
  std::vector<IntervaledOperation*> ops;

public:
  IntervalExecutor(const std::initializer_list<IntervaledOperation*> ops);

  void spin();
  void wait_and_execute();
};

#endif
