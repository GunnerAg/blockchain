#include "BlockClient.h"

#include <sstream>

BlockClient::BlockClient() {}

void BlockClient::SendBlock(const Block &block)
{
  // Serialize the block
  std::string data = block.Serialize();

  // Send the block to the server
  Send(data);
}

Block BlockClient::ReceiveBlock()
{
  // Receive the block from the server
  std::string data = Receive();

  // Deserialize the block
  return Block::Deserialize(data);
}
