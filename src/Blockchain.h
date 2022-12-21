#pragma once

#include <cstdint>
#include <vector>

#include "Block.h"

class Blockchain
{
public:
  // Constructs a new blockchain with a genesis block
  Blockchain();

  // Returns the current list of blocks in the blockchain
  const std::vector<Block> &blocks() const { return blocks_; }

  // Adds a new block to the blockchain
  void AddBlock(const Block &block);

  // Mines a new block with the given data and adds it to the blockchain
  void Mine(const std::vector<Transaction> &transactions);

  // Returns the balance for the given address
  int GetBalance(const std::string &address) const;

  // Returns true if the blockchain is valid, false otherwise
  bool IsValid() const;

private:
  // The list of blocks in the blockchain
  std::vector<Block> blocks_;
};
