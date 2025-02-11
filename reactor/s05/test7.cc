#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include <stdio.h>

void newConnection(int sockfd, const muduo::InetAddress& peerAddr)
{
  printf("newConnection(): accepted a new connection from %s\n",
         peerAddr.toHostPort().c_str());
  ::write(sockfd, "How are you?\n", 13);
  muduo::sockets::close(sockfd);
}

// int main()
// {
//   printf("main(): pid = %d\n", getpid());

//   muduo::InetAddress listenAddr(9981);
//   muduo::EventLoop loop;

//   muduo::Acceptor acceptor(&loop, listenAddr);
//   acceptor.setNewConnectionCallback(newConnection);
//   acceptor.listen();

//   loop.loop();
// }

int main() {
  printf("main(): pid = %d\n", getpid());
  {
    muduo::EventLoopThread loopThread;
    muduo::EventLoop* loop = loopThread.startLoop();
    muduo::InetAddress listenAddr(9981);
    muduo::Acceptor acceptor(loop, listenAddr);
    acceptor.setNewConnectionCallback(newConnection);
    loop->runInLoop(std::bind(&muduo::Acceptor::listen, &acceptor));
  }

  sleep(1000);
  printf("main(): pid = %d exiting...\n", getpid());
  return 0;
}