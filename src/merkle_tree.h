#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <string>
#include <vector>
#include "transaction.h"

// A class representing a Merkle tree
class MerkleTree
{
public:
  // Constructs a Merkle tree from a list of transactions
  MerkleTree(const std::vector<Transaction> &transactions);

  // Verifies that the given root hash is the root hash of the given data
  bool Verify(const std::string &root_hash, const std::vector<std::string> &data);

  // Returns the root hash of the tree
  const std::string &GetRootHash() const;

  // Sets the root hash of the given block to the root hash of the tree
  void GetRootHash(Block &block);

private:
  // Calculates the root hash of the given data
  std::string CalculateRootHash(const std::vector<std::string> &data);

  // Converts a byte to a hexadecimal string
  std::string StringifyByte(unsigned char c);

  // The transactions in the tree
  std::vector<Transaction> transactions_;

  // The root hash of the tree
  std::string root_hash_;
};

#endif
