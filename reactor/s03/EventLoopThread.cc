// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "EventLoopThread.h"

#include "EventLoop.h"

#include "logging/Logging.h"

#include <boost/bind.hpp>

using namespace muduo;

EventLoopThread::EventLoopThread()
  : loop_(NULL),
    exiting_(false),
    thread_(boost::bind(&EventLoopThread::threadFunc, this)),
    mutex_(),
    cond_(mutex_)
{
  LOG_INFO << "EventLoopThread() begin and end...";
}

EventLoopThread::~EventLoopThread()
{
  LOG_INFO << "~EventLoopThread() begin...";
  exiting_ = true;
  loop_->quit();
  thread_.join();
  LOG_INFO << "~EventLoopThread() end...";
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!thread_.started());
  thread_.start();
  LOG_INFO << "EventLoopThread::startLoop [after] thread_.start()";
  {
    MutexLockGuard lock(mutex_);
    while (loop_ == NULL)
    {
      cond_.wait();
    }
  }
  LOG_INFO << "EventLoopThread::startLoop() wakeup from cond and return loop_";
  return loop_;
}

void EventLoopThread::threadFunc()
{
  LOG_INFO << "EventLoopThread::threadFunc begin...";
  EventLoop loop;

  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }

  LOG_INFO << "EventLoopThread::threadFunc [before] loop.loop()";
  loop.loop();

  LOG_INFO << "EventLoopThread::threadFunc end... exiting_=" << exiting_;
  //assert(exiting_);
}
