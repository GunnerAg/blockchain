#include <string>
#include <vector>
#include <openssl/sha.h>

#include "block.h"
#include "merkle_tree.h"
#include "transaction.h"

Block::Block(const std::vector<Transaction> &transactions, const std::string &previous_hash) : transactions_(transactions), previous_hash_(previous_hash)
{
  // Use the Merkle tree to verify the integrity of the transactions
  MerkleTree tree(transactions_);
  if (!tree.Verify())
  {
    throw std::invalid_argument("Invalid transactions in the block.");
  }

  // Calculate the hash of the block
  root_hash_ = tree.GetRootHash();
  std::string header = previous_hash_ + root_hash_;
  hash_ = CalculateHash(header);
}

const std::string &Block::GetHash() const
{
  return hash_;
}

const std::string &Block::GetPreviousHash() const
{
  return previous_hash_;
}

const std::vector<Transaction> &Block::GetTransactions() const
{
  return transactions_;
}

std::string Block::CalculateHash() const
{
  // Initialize the SHA-3 hash context
  SHA256_CTX ctx;
  SHA256_Init(&ctx);
  // Update the hash with the block header
  SHA256_Update(&ctx, &timestamp_, sizeof(timestamp_));
  SHA256_Update(&ctx, &previous_hash_, sizeof(previous_hash_));
  SHA256_Update(&ctx, &merkle_root_, sizeof(merkle_root_));
  SHA256_Update(&ctx, &nonce_, sizeof(nonce_));
  // Update the hash with the block body (transactions)
  for (const Transaction &transaction : transactions_)
  {
    SHA256_Update(&ctx, transaction.GetHash().c_str(), transaction.GetHash().size());
  }
  // Finalize the hash
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_Final(hash, &ctx);
  // Convert the hash to a hexadecimal string
  char hash_str[SHA256_DIGEST_LENGTH * 2 + 1];
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    sprintf(hash_str + i * 2, "%02x", hash[i]);
  }
  return std::string(hash_str);
}
