#include "TransactionClient.h"

#include <sstream>

TransactionClient::TransactionClient() {}

void TransactionClient::SendTransaction(const Transaction &transaction)
{
  // Serialize the transaction
  std::string data = transaction.Serialize();

  // Send the transaction to the server
  Send(data);
}

Transaction TransactionClient::ReceiveTransaction()
{
  // Receive the transaction from the server
  std::string data = Receive();

  // Deserialize the transaction
  return Transaction::Deserialize(data);
}
