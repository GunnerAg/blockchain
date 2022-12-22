#include <vector>
#include <string>
#include "blockchain.h"
#include "consensus.h"
#include "network.h"

class Node
{
public:
  // Constructor for creating a new node
  Node(const std::string &address, const Blockchain &blockchain, const Consensus &consensus, const Network &network);

  // Method for validating a new block
  bool ValidateBlock(const Block &block);

  // Method for validating a new transaction
  bool ValidateTransaction(const Transaction &transaction);

  // Method for adding a new transaction to the pool
  void AddTransaction(const Transaction &transaction);

  // Method for synchronizing the node's copy of the blockchain with the network
  void SyncBlockchain();

private:
  std::string address_;           // Network address of the node
  Blockchain blockchain_;         // Copy of the blockchain
  Consensus consensus_;           // Consensus algorithm
  Network network_;               // Network communication
  std::vector<Transaction> pool_; // Pool of unconfirmed transactions
  bool banned_;                   // Flag indicating whether the node is banned
  int stake_;                     // Stake of the node
};
