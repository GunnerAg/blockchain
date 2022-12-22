#include <vector>
#include <string>

class Block
{
public:
  // Constructor for creating a new block
  Block(const std::vector<Transaction> &transactions, const std::string &previous_hash);

  // Method for calculating the block's hash
  std::string CalculateHash() const;

  // Method for verifying the integrity of the block's transactions
  bool VerifyTransactions() const;

  // Method for serializing the block to a string
  std::string Serialize() const;

  // Method for deserializing the block from a string
  static Block Deserialize(const std::string &str);

  std::string root_hash; // Root hash of the transactions in the block

private:
  std::vector<Transaction> transactions_; // List of transactions in the block
  std::string previous_hash_;             // Hash of the previous block in the chain
  std::string hash_;                      // Hash of the current block
  int timestamp_;                         // Timestamp of when the block was created
};
