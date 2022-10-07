#include "EventLoop.h"
#include "EventLoopThread.h"
#include <stdio.h>

void runInThread()
{
  printf("runInThread(): pid = %d, tid = %d\n",
         getpid(), muduo::CurrentThread::tid());
}

int main()
{
  printf("main(): pid = %d, tid = %d\n",
         getpid(), muduo::CurrentThread::tid());

  muduo::EventLoopThread loopThread;
  LOG_INFO << "main loopThread.startLoop begin...";
  muduo::EventLoop* loop = loopThread.startLoop();
  LOG_INFO << "main loopThread.startLoop end...";

  loop->runInLoop(runInThread);
  sleep(1);
  loop->runAfter(2, runInThread);
  sleep(3);

  LOG_INFO << "main thread call loop->quit() being...";
  loop->quit();
  LOG_INFO << "main thread call loop->quit() end...";

  printf("****** exit main() ******\n");
}
