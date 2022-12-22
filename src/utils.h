#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>

class Utils
{
public:
  // Generate a random number within a given range
  static int RandomInt(int min, int max);
  // Generate a random string of a given length
  static std::string RandomString(size_t length);
  // Hash a string using SHA3
  static std::string HashString(const std::string &str);
  // Sign a message using a private key
  static std::string SignMessage(const std::string &private_key,
                                 const std::string &message);
  // Verify a signature using a public key
  static bool VerifySignature(const std::string &public_key,
                              const std::string &message,
                              const std::string &signature);
  // Convert a vector of bytes to a hex string
  static std::string VectorToHexString(const std::vector<unsigned char> &vec);
  // Convert a hex string to a vector of bytes
  static std::vector<unsigned char> HexStringToVector(const std::string &str);
};

#endif // UTILS_H_
