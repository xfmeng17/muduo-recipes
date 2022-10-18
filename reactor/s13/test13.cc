#include "EventLoop.h"
#include "EventLoopThread.h"
#include "TcpConnection.h"
#include "InetAddress.h"
#include "TcpClient.h"

#include "logging/Logging.h"

#include <boost/bind.hpp>

#include <utility>

#include <stdio.h>
#include <unistd.h>

std::string message = "s13_test13_tcpclient_on_connection_send_msg\n";

void onConnection(const muduo::TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): new connection [%s] from %s\n",
           conn->name().c_str(),
           conn->peerAddress().toHostPort().c_str());
    conn->send(message);
  }
  else
  {
    printf("onConnection(): connection [%s] is down\n",
           conn->name().c_str());
  }
}

void onMessage(const muduo::TcpConnectionPtr& conn,
               muduo::Buffer* buf,
               muduo::Timestamp receiveTime)
{
  printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
         buf->readableBytes(),
         conn->name().c_str(),
         receiveTime.toFormattedString().c_str());

  printf("onMessage(): [%s]\n", buf->retrieveAsString().c_str());
}

// int main()
// {
//   muduo::EventLoop loop;
//   muduo::InetAddress serverAddr("localhost", 9981);
//   muduo::TcpClient client(&loop, serverAddr);

//   client.setConnectionCallback(onConnection);
//   client.setMessageCallback(onMessage);
//   client.enableRetry();
//   client.connect();
//   loop.loop();
// }

int main() {
  muduo::EventLoopThread loopThread;
  muduo::EventLoop* loop = loopThread.startLoop();

  muduo::InetAddress serverAddr("localhost", 9981);
  muduo::TcpClient client(loop, serverAddr);
  client.setConnectionCallback(onConnection);
  client.setMessageCallback(onMessage);
  client.enableRetry();
  client.connect();

  sleep(1);

  LOG_INFO << "EventLoop ptr=" << loop << " client.connection.getLoop=" << client.connection()->getLoop();
  LOG_INFO << "connect ok, then try send message";
  // return 0;

  for (int i = 10; i < 20; i++) {
    muduo::TcpConnectionPtr conn = client.connection();
    std::string msg = "test13.client with i is " + std::to_string(i);
    LOG_INFO << msg;
    conn->send(msg);
    sleep(1);
  }

  sleep(1000);
  return 0;
}