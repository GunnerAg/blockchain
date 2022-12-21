#include "Transaction.h"

#include "Crypto.h"

#include <sstream>

Transaction::Transaction(const std::string &sender,
                         const std::string &recipient, int amount,
                         const std::string &signature)
    : sender_(sender),
      recipient_(recipient),
      amount_(amount),
      signature_(signature) {}

std::string Transaction::Serialize() const
{
  std::stringstream ss;
  ss << sender_ << ' ' << recipient_ << ' ' << amount_ << ' ' << signature_;
  return ss.str();
}

Transaction Transaction::Deserialize(const std::string &data)
{
  std::stringstream ss(data);
  std::string sender;
  std::string recipient;
  int amount;
  std::string signature;
  ss >> sender >> recipient >> amount >> signature;
  return Transaction(sender, recipient, amount, signature);
}

bool Transaction::IsValid() const
{
  // Verify the signature
  return Crypto::VerifySignature(sender_, Serialize(), signature_);
}
