#include <iostream>
#include <string>
#include <vector>
#include "blockchain.h"
#include "consensus.h"
#include "merkletree.h"
#include "network.h"
#include "node.h"
#include "transaction.h"

int main()
{
  // Create instances of the blockchain, consensus, network, and node classes
  Blockchain blockchain;
  Consensus consensus(100, 10); // 100 nodes, 10 validators
  Network network;
  Node node("127.0.0.1", blockchain, consensus, network);

  // Handle user input and command line arguments
  while (true)
  {
    std::string input;
    std::cout << "Enter a command (e.g. 'create_transaction', 'create_block', 'sync'): ";
    std::cin >> input;

    if (input == "create_transaction")
    {
      // Create a new transaction and add it to the pool
      std::vector<std::string> inputs;
      std::vector<std::string> outputs;
      int fee;
      std::cout << "Enter inputs (space-separated list): ";
      for (std::string input; std::cin >> input;)
      {
        inputs.push_back(input);
      }
      std::cout << "Enter outputs (space-separated list): ";
      for (std::string output; std::cin >> output;)
      {
        outputs.push_back(output);
      }
      std::cout << "Enter fee: ";
      std::cin >> fee;
      Transaction transaction(inputs, outputs, fee);
      if (node.ValidateTransaction(transaction))
      {
        node.AddTransaction(transaction);
        std::cout << "Transaction added to pool." << std::endl;
      }
      else
      {
        std::cout << "Invalid transaction." << std::endl;
      }
    }
    else if (input == "create_block")
    {
      // Check if there are enough transactions in the pool to create a new block
      std::vector<Transaction> transactions;
      if (node.GetTransactionPoolSize() >= 10)
      {
        for (int i = 0; i < 10; i++)
        {
          transactions.push_back(node.GetTransactionFromPool());
        }
      }
      else
      {
        std::cout << "Not enough transactions in the pool." << std::endl;
        continue;
      }

      // Use the Merkle tree to verify the integrity of the transactions
      MerkleTree tree(transactions);
      if (!tree.Verify())
      {
        std::cout << "Invalid transactions in the block." << std::endl;
        continue;
      }

      // Create a new block and try to add it to the blockchain
      std::string previous_hash = blockchain.GetLatestBlock().GetHash();
      Block block(transactions, previous_hash);
      if (node.ValidateBlock(block) && consensus.RequireConsensus(consensus.SelectValidators(), block))
      {
        blockchain.AddBlock(block);
        std::cout << "Block added to the blockchain." << std::endl;
      }
      else
      {
        std::cout << "Invalid block." << std::endl;
      }
    }
    else if (input == "sync")
    {
      // Synchronize the node's copy of the blockchain with the network
      node.SyncBlockchain();
      std::cout << "Blockchain synchronized with the network." << std::endl;
    }
    else
    {
      std::cout << "Invalid command." << std::endl;
    }
  }

  return 0;
}