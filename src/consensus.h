#ifndef CONSENSUS_H_
#define CONSENSUS_H_

#include <vector>

class Block;

class Consensus
{
public:
  Consensus(int num_nodes, int num_validators);

  static std::vector<Node> SelectValidators(const std::vector<Node> &nodes, int num_validators);
  bool RequireConsensus(const std::vector<int> &validators, const Block &block) const;
  void BanNode(int node_id);

private:
  int num_nodes_;
  int num_validators_;
};

#endif // CONSENSUS_H_
