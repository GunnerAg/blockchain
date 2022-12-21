#include "Client.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

Client::Client() : connection_(0) {}

void Client::Connect()
{
  // Create the socket
  connection_ = socket(AF_INET, SOCK_STREAM, 0);
  if (connection_ < 0)
  {
    std::cerr << "Error creating socket" << std::endl;
    return;
  }

  // Connect to the server
  sockaddr_in address;
  std::memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(5000);
  if (connect(connection_, reinterpret_cast<sockaddr *>(&address), sizeof(address)) < 0)
  {
    std::cerr << "Error connecting to server" << std::endl;
    return;
  }
}

void Client::Disconnect()
{
  // Close the connection
  close(connection_);
  connection_ = 0;
}

void Client::Send(const std::string &message)
{
  // Send the message to the server
  if (send(connection_, message.c_str(), message.size(), 0) < 0)
  {
    std::cerr << "Error sending message" << std::endl;
    return;
  }
}

std::string Client::Receive()
{
  // Receive a message from the server
  char buffer[1024];
  if (recv(connection_, buffer, 1024, 0) < 0)
  {
    std::cerr << "Error receiving message" << std::endl;
    return "";
  }
  return buffer;
}
