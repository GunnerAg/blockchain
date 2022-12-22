#include <string>

class Transaction
{
public:
  // Constructs a new transaction with the given sender, recipient, and amount.
  Transaction(int sender, int recipient, double amount);
  // Returns the sender of the transaction.
  int GetSender() const;
  // Returns the recipient of the transaction.
  int GetRecipient() const;
  // Returns the amount of the transaction.
  double GetAmount() const;
  // Returns the hash of the transaction.
  std::string GetHash() const;
  // Returns true if the transaction is valid, false otherwise.
  bool IsValid() const;

  static constexpr size_t kMaxTransactionSize = 1024;

private:
  // Generates a random hash for the transaction.
  std::string GenerateRandomHash() const;

  int sender_;
  int recipient_;
  double amount_;
  std::string hash_;
};
