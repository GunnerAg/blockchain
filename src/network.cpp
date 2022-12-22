#include <openssl/ssl.h>
#include <openssl/err.h>
#include <rpc/rpc.h>
#include "network.h"

Network::Network()
{
  // Initialize the OpenSSL library
  SSL_library_init();
  SSL_load_error_strings();
  OpenSSL_add_all_algorithms();
  // Initialize the RPC library
  rpc_nt_init();
}

void Network::BroadcastTransaction(const Transaction &transaction)
{
  // Serialize the transaction data
  std::string serialized_transaction = SerializeTransaction(transaction);
  // Connect to the RPC server of each node in the network
  for (const Node &node : nodes_)
  {
    SSL *ssl = ConnectToRpcServer(node.rpc_server_url, node.rpc_server_port);
    // Send the "addtransaction" RPC request
    SendRpcRequest(ssl, "addtransaction", serialized_transaction);
    // Wait for the response
    std::string response = ReceiveRpcResponse(ssl);
    // Parse the response
    if (response == "success")
    {
      // The node successfully added the transaction to its pool of unconfirmed transactions
    }
    else
    {
      // The node rejected the transaction
      // This could be because the transaction was invalid, or because the node already has the transaction in its pool
    }
    // Disconnect from the RPC server
    SSL_shutdown(ssl);
    SSL_free(ssl);
  }
}

SSL *Network::ConnectToRpcServer(const std::string &host, int port)
{
  SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
  if (ctx == nullptr)
  {
    perror("ERROR creating SSL context");
    return nullptr;
  }

  SSL *ssl = SSL_new(ctx);
  if (ssl == nullptr)
  {
    perror("ERROR creating SSL object");
    SSL_CTX_free(ctx);
    return nullptr;
  }

  // Look up the IP address of the host
  struct hostent *server = gethostbyname(host.c_str());
  if (server == nullptr)
  {
    perror("ERROR, no such host");
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return nullptr;
  }

  // Set up the sockaddr_in struct with the address of the server
  struct sockaddr_in serv_addr;
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(port);

  // Create a socket to connect to the RPC server
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("ERROR opening socket");
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return nullptr;
  }

  // Connect to the server
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("ERROR connecting");
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sockfd);
    return nullptr;
  }

  // Set the socket as the file descriptor for the SSL object
  SSL_set_fd(ssl, sockfd);

  // Perform the SSL/TLS handshake
  if (SSL_connect(ssl) != 1)
  {
    perror("ERROR performing SSL/TLS handshake");
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sockfd);
    return nullptr;
  }

  // Save the socket descriptor
  rpc_server_sockfd_ = sockfd;

  // Return the SSL object
  return ssl;
}

std::string Network::SendRpcRequest(SSL *ssl, const std::string &request)
{
  // Send the request to the RPC server
  int bytes_written = SSL_write(ssl, request.c_str(), request.size());
  if (bytes_written < 0)
  {
    perror("ERROR writing to socket");
    return "";
  }

  // Read the response from the RPC server
  std::string response;
  char buffer[kBufferSize];
  int bytes_read;
  do
  {
    bytes_read = SSL_read(ssl, buffer, kBufferSize - 1);
    if (bytes_read < 0)
    {
      perror("ERROR reading from socket");
      return "";
    }
    buffer[bytes_read] = '\0';
    response += buffer;
  } while (bytes_read == kBufferSize - 1);

  return response;
}

std::string Network::ReceiveRpcResponse(SSL *ssl)
{
  // Read the response from the RPC server
  std::string response;
  char buffer[kBufferSize];
  int bytes_read;
  do
  {
    bytes_read = SSL_read(ssl, buffer, kBufferSize - 1);
    if (bytes_read < 0)
    {
      perror("ERROR reading from socket");
      return "";
    }
    buffer[bytes_read] = '\0';
    response += buffer;
  } while (bytes_read == kBufferSize - 1);

  return response;
}

std::vector<Transaction> Network::GetUnconfirmedTransactions() const
{
  std::vector<Transaction> unconfirmed_transactions;
  // Connect to the RPC server of each node in the network
  for (const Node &node : nodes_)
  {
    SSL *ssl = ConnectToRpcServer(node.rpc_server_url, node.rpc_server_port);
    // Send the "getunconfirmedtransactions" RPC request
    SendRpcRequest(ssl, "getunconfirmedtransactions", "");
    // Wait for the response
    std::string serialized_transactions = ReceiveRpcResponse(ssl);
    // Parse the serialized transactions
    std::vector<Transaction> node_unconfirmed_transactions = DeserializeTransactions(serialized_transactions);
    // Add the transactions to the unconfirmed transactions vector
    unconfirmed_transactions.insert(unconfirmed_transactions.end(), node_unconfirmed_transactions.begin(), node_unconfirmed_transactions.end());
    // Disconnect from the RPC server
    SSL_shutdown(ssl);
    SSL_free(ssl);
  }
  return unconfirmed_transactions;
}

void Network::BroadcastBlock(const Block &block)
{
  // Serialize the block data
  std::string serialized_block = SerializeBlock(block);
  // Connect to the RPC server of each node in the network
  for (const Node &node : nodes_)
  {
    SSL *ssl = ConnectToRpcServer(node.rpc_server_url, node.rpc_server_port);
    // Send the "addblock" RPC request
    SendRpcRequest(ssl, "addblock", serialized_block);
    // Wait for the response
    std::string response = ReceiveRpcResponse(ssl);
    // Parse the response
    if (response == "success")
    {
      // The node successfully added the block to its blockchain
    }
    else
    {
      // The node rejected the block
      // This could be because the block was invalid, or because the node already has the block in its blockchain
    }
    // Disconnect from the RPC server
    SSL_shutdown(ssl);
    SSL_free(ssl);
  }
}

std::vector<Transaction> Network::GetUnconfirmedTransactions()
{
  // Connect to the RPC server
  SSL *ssl = ConnectToRpcServer(rpc_server_host_, rpc_server_port_);
  if (ssl == nullptr)
  {
    return {};
  }

  // Construct the RPC request
  std::string request = R"({"jsonrpc": "2.0", "method": "getunconfirmedtransactions", "params": [], "id": 0})";

  // Send the request and receive the response
  std::string response = SendRpcRequest(ssl, request);

  // Parse the JSON response
  Json::Value root;
  Json::Reader reader;
  bool parsing_successful = reader.parse(response, root);
  if (!parsing_successful)
  {
    std::cerr << "ERROR parsing JSON response: " << reader.getFormattedErrorMessages() << std::endl;
    return {};
  }

  // Check for error in the response
  if (root.isMember("error"))
  {
    std::cerr << "ERROR in RPC response: " << root["error"]["message"].asString() << std::endl;
    return {};
  }

  // Extract the unconfirmed transactions from the response
  std::vector<Transaction> unconfirmed_transactions;
  Json::Value transactions_json = root["result"];
  for (const auto &transaction_json : transactions_json)
  {
    Transaction transaction;
    transaction.from_json(transaction_json);
    unconfirmed_transactions.push_back(transaction);
  }

  return unconfirmed_transactions;
}

std::vector<Transaction> Network::DeserializeTransactions(const std::string &serialized)
{
  // Parse the serialized transactions
  Json::Value root;
  Json::Reader reader;
  bool parsing_successful = reader.parse(serialized, root);
  if (!parsing_successful)
  {
    std::cerr << "ERROR parsing serialized transactions: " << reader.getFormattedErrorMessages() << std::endl;
    return {};
  }

  // Extract the transactions from the serialized form
  std::vector<Transaction> transactions;
  for (const auto &transaction_json : root)
  {
    Transaction transaction;
    transaction.from_json(transaction_json);
    transactions.push_back(transaction);
  }

  return transactions;
}

std::vector<Block> Network::DeserializeBlocks(const std::string &serialized)
{
  // Parse the serialized blocks
  Json::Value root;
  Json::Reader reader;
  bool parsing_successful = reader.parse(serialized, root);
  if (!parsing_successful)
  {
    std::cerr << "ERROR parsing serialized blocks: " << reader.getFormattedErrorMessages() << std::endl;
    return {};
  }

  // Extract the blocks from the serialized form
  std::vector<Block> blocks;
  for (const auto &block_json : root)
  {
    Block block;
    block.from_json(block_json);
    blocks.push_back(block);
  }

  return blocks;
}

std::vector<Block> Network::GetBlockchain() const
{
  std::vector<Block> blockchain;
  // Connect to the RPC server of each node in the network
  for (const Node &node : nodes_)
  {
    SSL *ssl = ConnectToRpcServer(node.rpc_server_url, node.rpc_server_port);
    // Send the "getblockchain" RPC request
    SendRpcRequest(ssl, "getblockchain", "");
    // Wait for the response
    std::string serialized_blocks = ReceiveRpcResponse(ssl);
    // Parse the serialized blocks
    std::vector<Block> node_blockchain = DeserializeBlocks(serialized_blocks);
    // Add the blocks to the blockchain vector
    blockchain.insert(blockchain.end(), node_blockchain.begin(), node_blockchain.end());
    // Disconnect from the RPC server
    SSL_shutdown(ssl);
    SSL_free(ssl);
  }
  return blockchain;
}
