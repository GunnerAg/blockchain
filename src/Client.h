#pragma once

#include <string>

class Client
{
public:
  // Constructs a new client
  Client();

  // Connects to the server
  void Connect();

  // Disconnects from the server
  void Disconnect();

  // Sends a message to the server
  void Send(const std::string &message);

  // Receives a message from the server
  std::string Receive();

private:
  // The connection to the server
  int connection_;
};
