#pragma once

#include <string>

class Transaction
{
public:
  // Constructs a new transaction with the given data
  Transaction(const std::string &sender, const std::string &recipient,
              int amount, const std::string &signature);

  // Returns the transaction's sender
  const std::string &sender() const { return sender_; }

  // Returns the transaction's recipient
  const std::string &recipient() const { return recipient_; }

  // Returns the transaction's amount
  int amount() const { return amount_; }

  // Returns the transaction's signature
  const std::string &signature() const { return signature_; }

  // Serializes the transaction to a string
  std::string Serialize() const;

  // Deserializes the transaction from a string
  static Transaction Deserialize(const std::string &data);

  // Returns true if the transaction is valid, false otherwise
  bool IsValid() const;

private:
  // The transaction's sender
  std::string sender_;

  // The transaction's recipient
  std::string recipient_;

  // The transaction's amount
  int amount_;

  // The transaction's signature
  std::string signature_;
};
