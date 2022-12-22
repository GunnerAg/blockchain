#include "utils.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <random>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

int Utils::RandomInt(int min, int max)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(min, max);
  return dis(gen);
}

std::string Utils::RandomString(size_t length)
{
  static const char kAlphabet[] =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::string str(length, ' ');
  for (size_t i = 0; i < length; ++i)
  {
    str[i] = kAlphabet[RandomInt(0, sizeof(kAlphabet) - 2)];
  }
  return str;
}

std::string Utils::HashString(const std::string &str)
{
  unsigned char hash[SHA3_256_DIGEST_LENGTH];
  SHA3_256(reinterpret_cast<const unsigned char *>(str.c_str()), str.size(),
           hash);
  return VectorToHexString(
      std::vector<unsigned char>(hash, hash + SHA3_256_DIGEST_LENGTH));
}

std::string Utils::SignMessage(const std::string &private_key,
                               const std::string &message)
{
  // Convert the private key string to a BIO
  BIO *bio = BIO_new_mem_buf(private_key.c_str(), private_key.size());
  // Read the private key from the BIO
  RSA *rsa = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, nullptr);
  // Free the BIO
  BIO_free(bio);
  // Compute the signature
  unsigned char signature[RSA_size(rsa)];
  unsigned int signature_length;
  RSA_sign(NID_sha3_256, reinterpret_cast<const unsigned char *>(message.c_str()),
           message.size(), signature, &signature_length, rsa);
  // Free the RSA key
  RSA_free(rsa);
  // Return the signature as a hex string
  return VectorToHexString(
      std::vector<unsigned char>(signature, signature + signature_length));
}

bool Utils::VerifySignature(const std::string &public_key,
                            const std::string &message,
                            const std::string &signature)
{
  // Convert the public key string to a BIO
  BIO *bio = BIO_new_mem_buf(public_key.c_str(), public_key.size());
  // Read the public key from the BIO
  RSA *rsa = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
  // Free the BIO
  BIO_free(bio);
  // Convert the signature hex string to a vector of bytes
  std::vector<unsigned char> sig = HexStringToVector(signature);
  // Verify the signature
  int result = RSA_verify(NID_sha3_256,
                          reinterpret_cast<const unsigned char *>(message.c_str()),
                          message.size(), sig.data(), sig.size(), rsa);
  // Free the RSA key
  RSA_free(rsa);
  // Return the result of the verification
  return result == 1;
}

std::string Utils::VectorToHexString(
    const std::vector<unsigned char> &vec)
{
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (unsigned char c : vec)
  {
    ss << std::setw(2) << static_cast<int>(c);
  }
  return ss.str();
}

std::vector<unsigned char> Utils::HexStringToVector(const std::string &str)
{
  std::vector<unsigned char> vec;
  for (size_t i = 0; i < str.size(); i += 2)
  {
    std::string byte_str = str.substr(i, 2);
    unsigned char byte = static_cast<unsigned char>(std::stoul(byte_str, nullptr, 16));
    vec.push_back(byte);
  }
  return vec;
}
