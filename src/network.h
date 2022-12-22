#include <vector>
#include "block.h"
#include "transaction.h"

class Network
{
public:
  // Constructs a new network object.
  Network();
  // Broadcasts the given transaction to the network.
  void BroadcastTransaction(const Transaction &transaction);
  // Broadcasts the given block to the network.
  void BroadcastBlock(const Block &block);
  // Returns the pool of unconfirmed transactions from the network.
  std::vector<Transaction> GetUnconfirmedTransactions() const;
  // Returns the blockchain from the network.
  std::vector<Block> GetBlockchain() const;
};
