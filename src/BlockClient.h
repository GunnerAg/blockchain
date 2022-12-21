#pragma once

#include "Block.h"
#include "Client.h"

class BlockClient : public Client
{
public:
  // Constructs a new block client
  BlockClient();

  // Sends a block to the server
  void SendBlock(const Block &block);

  // Receives a block from the server
  Block ReceiveBlock();
};
