#include <openssl/ec.h>
#include <openssl/pem.h>

#include <iostream>

int main()
{
  // Generate a new private key
  EC_KEY *key = EC_KEY_new_by_curve_name(NID_secp256k1);
  EC_KEY_generate_key(key);

  // Write the private key to a file
  BIO *bio = BIO_new_file("client.key", "w");
  PEM_write_bio_ECPrivateKey(bio, key, nullptr, nullptr, 0, nullptr, nullptr);
  BIO_free(bio);

  // Generate a self-signed certificate
  X509 *x509 = X509_new();
  X509_set_version(x509, 2);
  ASN1_INTEGER_set(X509_get_serialNumber(x509), 0);
  X509_gmtime_adj(X509_get_notBefore(x509), 0);
  X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);
  X509_set_pubkey(x509, key);
  X509_NAME *name = X509_get_subject_name(x509);
  X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
                             reinterpret_cast<const unsigned char *>("client"), -1, -1, 0);
  X509_set_issuer_name(x509, name);
  X509_sign(x509, key, EVP_sha256());

  // Write the certificate to a file
  bio = BIO_new_file("client.crt", "w");
  PEM_write_bio_X509(bio, x509);
  BIO_free(bio);

  // Clean up
  EC_KEY_free(key);
  X509_free(x509);

  return 0;
}
