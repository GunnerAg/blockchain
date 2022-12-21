#include "Blockchain.h"

#include <algorithm>

Blockchain::Blockchain()
{
  // Add the genesis block
  blocks_.push_back(Block(0, {}, ""));
}

void Blockchain::AddBlock(const Block &block)
{
  // Make sure the block is valid
  if (!block.IsValid())
  {
    return;
  }

  // Make sure the block is a child of the last block
  if (block.previous_hash() != blocks_.back().hash())
  {
    return;
  }

  // Add the block to the list
  blocks_.push_back(block);
}

void Blockchain::Mine(const std::vector<Transaction> &transactions)
{
  // Create the new block
  Block block(blocks_.size(), transactions, blocks_.back().hash());

  // Add the block to the blockchain
  AddBlock(block);
}

int Blockchain::GetBalance(const std::string &address) const
{
  int balance = 0;
  for (const Block &block : blocks_)
  {
    for (const Transaction &transaction : block.transactions())
    {
      if (transaction.recipient() == address)
      {
        balance += transaction.amount();
      }
      if (transaction.sender() == address)
      {
        balance -= transaction.amount();
      }
    }
  }
  return balance;
}

bool Blockchain::IsValid() const
{
  // Check the genesis block
  if (blocks_[0].index() != 0 || blocks_[0].previous_hash() != "")
  {
    return false;
  }

  // Check the other blocks
  for (size_t i = 1; i < blocks_.size(); i++)
  {
    const Block &block = blocks_[i];
    if (block.index() != i || block.previous_hash() != blocks_[i - 1].hash())
    {
      return false;
    }
  }
  return true;
}
