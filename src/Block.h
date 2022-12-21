#pragma once

#include <cstdint>
#include <vector>

#include "Transaction.h"

class Block
{
public:
  // Constructs a new block with the given data and previous hash
  Block(uint32_t index, const std::vector<Transaction> &transactions,
        const std::string &previous_hash);

  // Returns the block's index
  uint32_t index() const { return index_; }

  // Returns the block's list of transactions
  const std::vector<Transaction> &transactions() const { return transactions_; }

  // Returns the block's previous hash
  const std::string &previous_hash() const { return previous_hash_; }

  // Returns the block's hash
  const std::string &hash() const { return hash_; }

  // Returns the block's timestamp
  uint64_t timestamp() const { return timestamp_; }

  // Serializes the block to a string
  std::string Serialize() const;

  // Deserializes the block from a string
  static Block Deserialize(const std::string &data);

  // Returns true if the block is valid, false otherwise
  bool IsValid() const;

private:
  // The block's index
  uint32_t index_;

  // The block's list of transactions
  std::vector<Transaction> transactions_;

  // The block's previous hash
  std::string previous_hash_;

  // The block's hash
  std::string hash_;

  // The block's timestamp
  uint64_t timestamp_;
};
