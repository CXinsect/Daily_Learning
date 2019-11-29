#ifndef MUDUO_NET_TIMERID_H
#define MUDUO_NET_TIMERID_H

#include <iostream>
#include <memory>

using namespace std;

namespace muduo
{

class Timer;

///
/// An opaque identifier, for canceling Timer.
///
class TimerId
{
 public:
  TimerId()
    : timer_(nullptr),
      sequence_(0)
  {
  }

  TimerId(unique_ptr<Timer> timer, int64_t seq)
    : timer_(timer.release()),
      sequence_(seq)
  {
  }
  TimerId(unique_ptr<Timer> timer): timer_(timer.release()), sequence_(0) {}

  friend class TimerQueue;

 private:
  unique_ptr<Timer> timer_;
  int64_t sequence_;
};

}  // namespace muduo

#endif 