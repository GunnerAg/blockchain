#include <map>
#include <set>
#include <vector>
#include "block.h"
#include "consensus.h"
#include "merkletree.h"
#include "network.h"
#include "node.h"
#include "transaction.h"

Node::Node(int id, const Consensus &consensus, const Network &network) : id_(id), consensus_(consensus), network_(network)
{
  // Load the blockchain from disk or initialize it if it does not exist
  blockchain_ = LoadBlockchain();
  if (blockchain_.empty())
  {
    InitializeBlockchain();
  }
}

void Node::ProcessTransaction(const Transaction &transaction)
{
  // Validate the transaction
  if (!transaction.IsValid())
  {
    return;
  }
  // Check if the transaction is already in the pool of unconfirmed transactions
  if (unconfirmed_transactions_.count(transaction.GetHash()) > 0)
  {
    return;
  }
  // Check if the transaction is already included in the blockchain
  if (IsTransactionInBlockchain(transaction))
  {
    return;
  }
  // Add the transaction to the pool of unconfirmed transactions
  unconfirmed_transactions_[transaction.GetHash()] = transaction;
  // Broadcast the transaction to the network
  network_.BroadcastTransaction(transaction);
}

void Node::ProcessBlock(const Block &block)
{
  // Validate the block
  if (!block.IsValid())
  {
    return;
  }
  // Check if the block is already in the blockchain
  if (IsBlockInBlockchain(block))
  {
    return;
  }
  // Check if the block is valid according to the consensus rules
  if (!consensus_.RequireConsensus(consensus_.SelectValidators(), block))
  {
    return;
  }
  // Add the block to the blockchain
  blockchain_.push_back(block);
  // Remove the included transactions from the pool of unconfirmed transactions
  for (const Transaction &transaction : block.GetTransactions())
  {
    unconfirmed_transactions_.erase(transaction.GetHash());
  }
  // Broadcast the block to the network
  network_.BroadcastBlock(block);
}

const std::vector<Block> &Node::GetBlockchain() const
{
  return blockchain_;
}

const std::map<std::string, Transaction> &Node::GetUnconfirmedTransactions() const
{
  return unconfirmed_transactions_;
}

void Node::LoadBlockchain(const std::vector<Block> &blockchain)
{
  // Validate each block in the blockchain
  for (const Block &block : blockchain)
  {
    // Validate the transactions in the block
    if (!Consensus::ValidateTransactions(block))
    {
      // Discard the blockchain if any block has invalid transactions
      return;
    }
    // Check if the node agrees on the block
    if (!AgreeOnBlock(block))
    {
      // Discard the blockchain if the node disagrees on any block
      return;
    }
  }
  // If the blockchain is valid, set it as the node's blockchain
  blockchain_ = blockchain;
}

void Node::InitializeBlockchain()
{
  static bool initialized = false;
  if (initialized)
  {
    // Return if the blockchain has already been initialized
    return;
  }
  // Create the first block in the blockchain (the "genesis block")
  Block genesis_block;
  genesis_block.index = 0;
  genesis_block.timestamp = std::time(nullptr);
  // Calculate the root hash of the transactions in the genesis block
  MerkleTree tree(genesis_block.transactions);
  tree.GetRootHash(genesis_block);
  // Set the previous hash of the genesis block to all zeros
  genesis_block.previous_hash = std::string(64, '0');
  // Set the nonce of the genesis block to a random value
  genesis_block.nonce = std::rand();
  // Add the genesis block to the blockchain
  blockchain_.push_back(genesis_block);
  initialized = true;
}

bool Node::IsTransactionInBlockchain(const Transaction &transaction) const
{
  // Check if the transaction is included in any block in the blockchain
  for (const Block &block : blockchain_)
  {
    if (block.HasTransaction(transaction))
    {
      return true;
    }
  }
  return false;
}

bool Node::IsBlockInBlockchain(const Block &block) const
{
  // Check if the block is already in the blockchain
  return std::find(blockchain_.begin(), blockchain_.end(), block) != blockchain_.end();
}
