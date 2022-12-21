#include "Blockchain.h"
#include "BlockClient.h"
#include "TransactionClient.h"

#include <iostream>

int main()
{
  // Create a new blockchain
  Blockchain blockchain;

  // Create a transaction client
  TransactionClient transaction_client;

  // Connect to the transaction server
  transaction_client.Connect();

  // Send and receive a transaction
  Transaction transaction = transaction_client.ReceiveTransaction();
  std::cout << "Received transaction: " << transaction << std::endl;
  transaction_client.SendTransaction(transaction);

  // Disconnect from the transaction server
  transaction_client.Disconnect();

  // Mine the transaction
  blockchain.Mine(transaction);

  // Create a block client
  BlockClient block_client;

  // Connect to the block server
  block_client.Connect();

  // Send and receive a block
  Block block = block_client.ReceiveBlock();
  std::cout << "Received block: " << block << std::endl;
  block_client.SendBlock(block);

  // Disconnect from the block server
  block_client.Disconnect();

  // Add the block to the blockchain
  blockchain.AddBlock(block);

  return 0;
}
