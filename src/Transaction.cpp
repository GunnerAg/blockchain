#include <openssl/sha.h>
#include "transaction.h"

Transaction::Transaction(int sender, int recipient, double amount) : sender_(sender), recipient_(recipient), amount_(amount)
{
  // Generate a random hash for the transaction
  hash_ = GenerateRandomHash();
}

int Transaction::GetSender() const
{
  return sender_;
}

int Transaction::GetRecipient() const
{
  return recipient_;
}

double Transaction::GetAmount() const
{
  return amount_;
}

std::string Transaction::GetHash() const
{
  return hash_;
}

bool Transaction::IsValid() const
{
  // Check that the transaction has a valid size
  if (size() > kMaxTransactionSize)
  {
    return false;
  }
  // Check that the transaction has a valid signature
  if (!VerifySignature())
  {
    return false;
  }
  // Check that the transaction is not a double spend
  if (IsDoubleSpend())
  {
    return false;
  }
  // The transaction is valid if it passes all checks
  return true;
}

bool Transaction::VerifySignature() const
{
  // Use the public key of the sender to verify the signature
  return Utils::VerifySignature(sender_public_key_, data_, signature_);
}

bool Transaction::IsDoubleSpend(const std::vector<Transaction> &transactions) const
{
  // Check if the transaction has already been spent in the given list of transactions
  for (const Transaction &t : transactions)
  {
    if (t.sender_ == sender_ && t.amount_ == amount_ && t.hash_ == hash_)
    {
      // The transaction has already been spent
      return true;
    }
  }
  // The transaction has not been spent
  return false;
}
{
  // Check if the transaction has already been processed
  return processed_;
}

std::string Transaction::GenerateRandomHash() const
{
  // Initialize the SHA-3 hash context
  SHA256_CTX ctx;
  SHA256_Init(&ctx);
  // Update the hash with the transaction data
  SHA256_Update(&ctx, &sender_, sizeof(sender_));
  SHA256_Update(&ctx, &recipient_, sizeof(recipient_));
  SHA256_Update(&ctx, &amount_, sizeof(amount_));
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
