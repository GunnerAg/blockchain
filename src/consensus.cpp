#include <random>
#include <vector>
#include <unordered_map>

#include "block.h"
#include "consensus.h"
#include "node.h"

Consensus::Consensus(int num_nodes, int num_validators) : num_nodes_(num_nodes), num_validators_(num_validators)
{
  if (num_validators_ < 1 || num_validators_ > num_nodes_ / 2)
  {
    throw std::invalid_argument("Number of validators must be between 1 and half the number of nodes.");
  }
}

bool Consensus::ValidateTransactions(const Block &block)
{
  // Calculate the root hash of the transactions in the block
  MerkleTree tree(block.transactions);
  std::string calculated_root_hash = tree.CalculateRootHash();
  // Compare the calculated root hash with the root hash in the block header
  return calculated_root_hash == block.root_hash;
}

std::vector<Node> Consensus::SelectValidators(const std::vector<Node> &nodes, int num_validators)
{
  // Map each node to its stake
  std::unordered_map<Node, int> node_stakes;
  for (const Node &node : nodes)
  {
    node_stakes[node] = node.stake;
  }
  // Sort the nodes by their stake
  std::vector<Node> sorted_nodes(nodes.begin(), nodes.end());
  std::sort(sorted_nodes.begin(), sorted_nodes.end(), [&](const Node &a, const Node &b)
            { return node_stakes[a] > node_stakes[b]; });
  // Create a vector of weights for each node
  std::vector<int> weights;
  for (const Node &node : sorted_nodes)
  {
    weights.push_back(node_stakes[node]);
  }
  // Create a random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  // Create a discrete distribution using the weights as probabilities
  std::discrete_distribution<> dist(weights.begin(), weights.end());
  // Choose 10% of the total nodes as validators
  int total_nodes = nodes.size();
  int num_validators = total_nodes / 10;
  std::vector<Node> validators;
  for (int i = 0; i < num_validators; ++i)
  {
    validators.push_back(sorted_nodes[dist(gen)]);
  }
  return validators;
}

bool Consensus::RequireConsensus(const std::vector<Node> &validators, const Block &block)
{
  // Count the number of validators that agree on the block
  int num_agreements = 0;
  for (const Node &validator : validators)
  {
    if (validator.AgreeOnBlock(block))
    {
      ++num_agreements;
    }
    else
    {
      // Ban the validator if it tries to manipulate the block
      validator.BanNode();
    }
  }

  if (!ValidateTransactions(block))
  {
    return false;
  }

  // Return true if at least 51% of the validators agree on the block
  return num_agreements * 2 > validators.size();
}

void Node::BanNode()
{
  // Mark the node as banned
  banned_ = true;
  // Lose the stake
  stake_ = 0;
}
