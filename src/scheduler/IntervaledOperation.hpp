#ifndef INTERVALED_OPERATION_HPP
#define INTERVALED_OPERATION_HPP

#include <ctime>
#include <cstdint>
#include <chrono>

#include "../gmcapi/GmcServer.hpp"

using namespace std::chrono;

class IntervaledOperation {
private:
  system_clock::duration m_interval;
  system_clock::time_point m_next_timestamp;

protected:
  virtual void operation() = 0;

public:
  IntervaledOperation(
    const system_clock::duration interval,
    system_clock::time_point next_timestamp
  );

  IntervaledOperation(const system_clock::duration interval);

  void run_and_update();

  system_clock::time_point next_timestamp();
};

class DailyEnvIntervalServerOperation: public IntervaledOperation {
private:
  virtual void operation() = 0;

protected:
  GmcServer* m_server;

public:
  DailyEnvIntervalServerOperation(GmcServer* server, const char* env_name);
};

#endif
