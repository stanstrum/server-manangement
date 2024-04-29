#ifndef INTERVALED_OPERATION_HPP
#define INTERVALED_OPERATION_HPP

#include <ctime>
#include <cstdint>

class IntervaledOperation {
private:
  const time_t interval;
  time_t next_timestamp;

protected:
  virtual void operation() = 0;

public:
  IntervaledOperation(
    const time_t interval,
    time_t next_timestamp
  );

  IntervaledOperation(const time_t interval);

  void run_and_update();
  time_t seconds_until();
};

#endif
