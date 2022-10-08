#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include <stdio.h>

#include <thread>

void newConnection1(int sockfd, const muduo::InetAddress& peerAddr)
{
  printf("tid = %d, newConnection1(): accepted a new connection from %s\n",
         muduo::CurrentThread::tid(), peerAddr.toHostPort().c_str());
  ::write(sockfd, "How are you?\n", 13);
  muduo::sockets::close(sockfd);
}

void newConnection2(int sockfd, const muduo::InetAddress& peerAddr)
{
  printf("tid = %d, newConnection2(): accepted a new connection from %s\n",
         muduo::CurrentThread::tid(), peerAddr.toHostPort().c_str());
  std::string msg = "Who are yout???\n";
  ::write(sockfd, msg.c_str(), msg.size());
  muduo::sockets::close(sockfd);
}

void newConnection3(int sockfd, const muduo::InetAddress& peerAddr)
{
  printf("tid = %d, newConnection3(): accepted a new connection from %s\n",
         muduo::CurrentThread::tid(), peerAddr.toHostPort().c_str());
  std::string result = "newConnection3 is \n";
  ::write(sockfd, result.c_str(), result.size());
  muduo::sockets::close(sockfd);
}

void newConnection4(int sockfd, const muduo::InetAddress& peerAddr)
{
  printf("tid = %d, newConnection4(): accepted a new connection from %s\n",
         muduo::CurrentThread::tid(), peerAddr.toHostPort().c_str());
  std::string result = "newConnection4 is \n";
  ::write(sockfd, result.c_str(), result.size());
  muduo::sockets::close(sockfd);
}

// int main()
// {
//   printf("main(): tid = %d starting...\n", muduo::CurrentThread::tid());

//   size_t loop_num = 3;
//   std::thread threads[loop_num];
//   threads[0] = std::thread([](){
//     muduo::InetAddress listenAddr(9981);
//     muduo::EventLoop loop;
//     muduo::Acceptor acceptor(&loop, listenAddr);
//     acceptor.setNewConnectionCallback(newConnection1);
//     acceptor.listen();
//     loop.loop();
//   });
//   threads[1] = std::thread([](){
//     muduo::InetAddress listenAddr(9982);
//     muduo::EventLoop loop;
//     muduo::Acceptor acceptor(&loop, listenAddr);
//     acceptor.setNewConnectionCallback(newConnection2);
//     acceptor.listen();
//     loop.loop();
//   });
//   threads[2] = std::thread([](){
//     muduo::InetAddress listenAddr(9983);
//     muduo::EventLoop loop;
//     muduo::Acceptor acceptor(&loop, listenAddr);
//     acceptor.setNewConnectionCallback(newConnection3);
//     acceptor.listen();
//     loop.loop();
//   });

//   for (size_t i = 0; i < loop_num; i++) {
//     threads[i].join();
//   }
//   printf("main(): tid = %d exiting...\n", muduo::CurrentThread::tid());
//   return 0;
// }

int main()
{
  printf("main(): tid = %d starting...\n", muduo::CurrentThread::tid());

  muduo::EventLoopThread loopThread;
  muduo::EventLoop* p_loop = loopThread.startLoop();
  muduo::InetAddress listenAddr4(9984);
  muduo::Acceptor acceptor4(p_loop, listenAddr4);
  acceptor4.setNewConnectionCallback(newConnection4);
  p_loop->runInLoop(std::bind(&muduo::Acceptor::listen, &acceptor4));

  printf("main(): tid = %d finish loopThread for 4\n", muduo::CurrentThread::tid());

  muduo::EventLoop loop;

  muduo::InetAddress listenAddr1(9981);
  muduo::Acceptor acceptor1(&loop, listenAddr1);
  acceptor1.setNewConnectionCallback(newConnection1);
  acceptor1.listen();

  muduo::InetAddress listenAddr2(9982);
  muduo::Acceptor acceptor2(&loop, listenAddr2);
  acceptor2.setNewConnectionCallback(newConnection2);
  acceptor2.listen();

  muduo::InetAddress listenAddr3(9983);
  muduo::Acceptor acceptor3(&loop, listenAddr3);
  acceptor3.setNewConnectionCallback(newConnection3);
  acceptor3.listen();

  loop.loop();
  printf("main(): tid = %d exiting...\n", muduo::CurrentThread::tid());
  return 0;
}