#include <string>
#include <vector>
#include "merkletree.h"
#include "transaction.h"

MerkleTree::MerkleTree(const std::vector<Transaction> &transactions) : transactions_(transactions)
{
  // Calculate the root hash of the tree
  root_hash_ = CalculateRootHash(transactions_);
}

bool MerkleTree::Verify(const std::string &root_hash, const std::vector<std::string> &data)
{
  // Calculate the root hash of the data
  std::string calculated_root_hash = CalculateRootHash(data);
  // Compare the calculated root hash to the given root hash
  return calculated_root_hash == root_hash;
}

const std::string &MerkleTree::GetRootHash() const
{
  return root_hash_;
}

void MerkleTree::GetRootHash(Block &block)
{
  block.root_hash = CalculateRootHash();
}

std::string MerkleTree::CalculateRootHash(const std::vector<std::string> &data)
{
  // Initialize the SHA3 context
  SHA3_CTX ctx;
  SHA3_256_Init(&ctx);
  // Hash each piece of data
  for (const std::string &d : data)
  {
    SHA3_Update(&ctx, d.data(), d.size());
  }
  // Finalize the hash
  unsigned char hash[SHA3_256_DIGEST_LENGTH];
  SHA3_256_Final(hash, &ctx);
  // Convert the hash to a hexadecimal string
  std::string root_hash = "";
  for (unsigned char c : hash)
  {
    root_hash += StringifyByte(c);
  }
  return root_hash;
}

std::string MerkleTree::StringifyByte(unsigned char c)
{
  static const char *hex_chars = "0123456789abcdef";
  std::string s;
  s += hex_chars[c >> 4];
  s += hex_chars[c & 0xf];
  return s;
}
