// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_NET_POLLER_H
#define MUDUO_NET_POLLER_H

#include <map>
#include <vector>

#include "datetime/Timestamp.h"
#include "EventLoop.h"

struct pollfd;

namespace muduo
{

class Channel;

///
/// IO Multiplexing with poll(2).
///
/// This class doesn't own the Channel objects.
class Poller : boost::noncopyable
{
 public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop* loop);
  ~Poller();

  /// Polls the I/O events.
  /// Must be called in the loop thread.
  Timestamp poll(int timeoutMs, ChannelList* activeChannels);

  /// Changes the interested I/O events.
  /// Must be called in the loop thread.
  void updateChannel(Channel* channel);

  void assertInLoopThread() { ownerLoop_->assertInLoopThread(); }

  const std::string toString() const;

 private:
  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;

  typedef std::vector<struct pollfd> PollFdList;
  typedef std::map<int, Channel*> ChannelMap;

  EventLoop* ownerLoop_;
  PollFdList pollfds_;
  ChannelMap channels_;
};

}
#endif  // MUDUO_NET_POLLER_H
