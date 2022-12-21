#pragma once

#include "Transaction.h"
#include "Client.h"

class TransactionClient : public Client
{
public:
  // Constructs a new transaction client
  TransactionClient();

  // Sends a transaction to the server
  void SendTransaction(const Transaction &transaction);

  // Receives a transaction from the server
  Transaction ReceiveTransaction();
};
