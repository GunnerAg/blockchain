#include "Block.h"
#include "sha256.h"

Block::Block(uint32_t index, const std::string &previous_hash, const std::string &merkle_root,
             uint32_t timestamp, uint32_t proof)
    : m_index(index), m_previous_hash(previous_hash), m_merkle_root(merkle_root),
      m_timestamp(timestamp), m_proof(proof)
{
  // Calculate the block hash
  std::stringstream ss;
  ss << index << previous_hash << merkle_root << timestamp << proof;
  m_hash = sha256(ss.str());
}

uint32_t Block::get_index() const
{
  return m_index;
}

std::string Block::get_previous_hash() const
{
  return m_previous_hash;
}

std::string Block::get_merkle_root() const
{
  return m_merkle_root;
}

uint32_t Block::get_timestamp() const
{
  return m_timestamp;
}

uint32_t Block::get_proof() const
{
  return m_proof;
}

std::string Block::get_hash() const
{
  return m_hash;
}
