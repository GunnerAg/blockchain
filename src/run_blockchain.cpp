#include <iostream>
#include <vector>
#include <memory>
#include "TransactionClient.h"
#include "BlockClient.h"
#include "Blockchain.h"

// Run the blockchain
void run(Blockchain &blockchain, std::vector<std::shared_ptr<Client>> &nodes)
{
  while (true)
  {
    // Check for new transactions from all the transaction clients
    for (const auto &node : nodes)
    {
      if (auto transaction_client = dynamic_cast<TransactionClient *>(node.get()))
      {
        // Check for new transactions
        while (true)
        {
          std::string transaction = transaction_client->receive();
          if (transaction.empty())
          {
            break;
          }

          // Validate the transaction
          if (transaction_client->validate_transaction(transaction))
          {
            // Add the transaction to the blockchain
            blockchain.add_transaction(transaction);

            // Propagate the transaction to the other nodes
            for (const auto &other_node : nodes)
            {
              if (other_node != node)
              {
                other_node->send(transaction);
              }
            }
          }
        }
      }
    }

    // Check for new blocks from all the block clients
    for (const auto &node : nodes)
    {
      if (auto block_client = dynamic_cast<BlockClient *>(node.get()))
      {
        // Check for new blocks
        while (true)
        {
          std::string block = block_client->receive();
          if (block.empty())
          {
            break;
          }

          // Validate the block
          if (block_client->validate_block(block))
          {
            // Add the block to the blockchain
            blockchain.add_block(block);

            // Propagate the block to the other nodes
            for (const auto &other_node : nodes)
            {
              if (other_node != node)
              {
                other_node->send(block);
              }
            }
          }
        }
      }
    }

    // Check if it's time to create a new block
    if (blockchain.should_create_new_block())
    {
      // Create a new block
      std::string new_block = blockchain.create_new_block();

      // Propagate the new block to the other nodes
      for (const auto &node : nodes)
      {
        node->send(new_block);
      }
    }

    // Wait for a moment before checking for new transactions and blocks again
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}
