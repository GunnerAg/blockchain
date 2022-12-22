#include <vector>
#include <string>

class MerkleTree
{
public:
  // Constructor for creating a new Merkle tree
  MerkleTree(const std::vector<Transaction> &transactions);

  // Method for generating the Merkle root from the tree
  std::string GetMerkleRoot() const;

private:
  // Recursive helper function for constructing the tree
  void BuildTree(const std::vector<Transaction> &transactions);

  std::vector<std::string> hashes_; // List of hashes in the tree
};
