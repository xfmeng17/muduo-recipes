#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include <stdio.h>

#include <thread>

void newConnection(int sockfd, const muduo::InetAddress& peerAddr)
{
  printf("newConnection(): accepted a new connection from %s\n",
         peerAddr.toHostPort().c_str());
  ::write(sockfd, "How are you?\n", 13);
  muduo::sockets::close(sockfd);
}

void newConnection2(int sockfd, const muduo::InetAddress& peerAddr)
{
  printf("newConnection2(): accepted a new connection from %s\n",
         peerAddr.toHostPort().c_str());
  std::string msg = "Who are yout???\n";
  ::write(sockfd, msg.c_str(), msg.size());
  muduo::sockets::close(sockfd);
}

void daytime(int sockfd, const muduo::InetAddress& peerAddr)
{
  printf("daytime(): accepted a new connection from %s\n",
         peerAddr.toHostPort().c_str());
  std::string result = "daytime is \n";
  ::write(sockfd, result.c_str(), result.size());
  muduo::sockets::close(sockfd);
}

int main()
{
  printf("main(): pid = %d starting...\n", getpid());

  size_t loop_num = 3;
  std::thread threads[loop_num];
  threads[0] = std::thread([](){
    muduo::InetAddress listenAddr(9981);
    muduo::EventLoop loop;
    muduo::Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback(newConnection);
    acceptor.listen();
    loop.loop();
  });
  threads[1] = std::thread([](){
    muduo::InetAddress listenAddr(9982);
    muduo::EventLoop loop;
    muduo::Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback(newConnection2);
    acceptor.listen();
    loop.loop();
  });
  threads[2] = std::thread([](){
    muduo::InetAddress listenAddr(9983);
    muduo::EventLoop loop;
    muduo::Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback(daytime);
    acceptor.listen();
    loop.loop();
  });

  for (size_t i = 0; i < loop_num; i++) {
    threads[i].join();
  }
  printf("main(): pid = %d exiting...\n", getpid());
  return 0;
}
