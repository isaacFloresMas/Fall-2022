#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
// clang-format off
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
// clang-format on

extern gmp_randstate_t state;

int main(int argc, char **argv) {
  int opt = 0;
  uint64_t bits = 1024;
  uint64_t iterations = 50;
  uint64_t seed = time(NULL);
  char *pbfile;
  char *pvfile;
  pbfile = "rsa.pub";
  pvfile = "rsa.priv";
  int verbose = 0;
  void print_help(void);
  while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
    switch (opt) {
    case 'b': {
      if ((bits = atoi(optarg)) < 50 || bits > 4096) {
        fprintf(stderr, "./keygen: Number of bits must be 50-4096, not %lu.\n",
                bits);
        print_help();
        return 1;
      }
      break;
    }
    case 'i': {
      if ((iterations = atoi(optarg)) < 1 || iterations > 500) {
        fprintf(stderr,
                "./keygen: Number of iterations must be 1-500, not %lu.\n.",
                iterations);
        print_help();
        return 1;
      }
      break;
    }
    case 'n': {
      pbfile = optarg;
      break;
    }
    case 'd': {
      pvfile = optarg;
      break;
    }
    case 's': {
      seed = strtoul(optarg, NULL, 0);
      break;
    }
    case 'v': {
      verbose = 1;
      break;
    }
    case 'h': {
      print_help();
      return 0;
    }
    }
  }
  FILE *fpub;
  FILE *fpriv;
  char *user = NULL;
  fpub = fopen(pbfile, "w+");  // open the public and private key file locations
  fpriv = fopen(pvfile, "w+"); // with read and write permissions
  if (fpub == NULL || fpriv == NULL) { // if there was an error, print an error message
    fprintf(stderr, "The public key file or private key file had trouble opening.\n");
    return 1;
  }
  fchmod(fileno(fpriv),
         S_IRUSR | S_IWUSR); // set private key file permissions to 0600
  mpz_t big_p, big_q, n_mod, exp, d_key, signature, message;
  mpz_inits(big_p, big_q, n_mod, exp, d_key, signature, message, NULL);
  randstate_init(seed); // initialize random state
  rsa_make_pub(big_p, big_q, n_mod, exp, bits,
               iterations); // make public and private key data
  rsa_make_priv(d_key, exp, big_p, big_q);
  user = getenv("USER"); // get username and convert it to an mpz_t
  mpz_set_str(message, user, 62);
  rsa_sign(signature, message, d_key, n_mod); // rsa sign the username
  rsa_write_pub(n_mod, exp, signature, user,
                fpub); // write the public and private keys to their files
  rsa_write_priv(n_mod, d_key, fpriv);

  if (verbose) { // if verbose is wanted, print verbose info
    void verbose_print(char *username, mpz_t signature, mpz_t p, mpz_t q,
                       mpz_t n, mpz_t e, mpz_t d);
    verbose_print(user, signature, big_p, big_q, n_mod, exp, d_key);
  }

  fclose(fpub);
  fclose(fpriv);
  mpz_clears(big_p, big_q, n_mod, exp, d_key, signature, message, NULL);
  randstate_clear();
  return 0;
}

void verbose_print(char *username, mpz_t signature, mpz_t p, mpz_t q, mpz_t n,
                   mpz_t e, mpz_t d) {
  gmp_fprintf(stderr, "username: %s\n", username);
  gmp_fprintf(stderr, "user signature (%d bits): %Zd\n",
              mpz_sizeinbase(signature, 2), signature);
  gmp_fprintf(stderr, "p (%d bits): %Zd\n", mpz_sizeinbase(p, 2), p);
  gmp_fprintf(stderr, "q (%d bits): %Zd\n", mpz_sizeinbase(q, 2), q);
  gmp_fprintf(stderr, "n - modulus (%d bits): %Zd\n", mpz_sizeinbase(n, 2), n);
  gmp_fprintf(stderr, "e - public exponent (%d bits): %Zd\n",
              mpz_sizeinbase(e, 2), e);
  gmp_fprintf(stderr, "d - private exponent (%d bits): %Zd\n",
              mpz_sizeinbase(d, 2), d);
}

void print_help(void) {
  gmp_fprintf(stderr, "Usage: ./keygen [options]\n");
  gmp_fprintf(stderr, "  ./keygen generates a public / private key pair, "
                      "placing the keys into the public and private\n");
  gmp_fprintf(stderr, "  key files as specified below. The keys have a modulus "
                      "(n) whose length is specified in\n");
  gmp_fprintf(stderr, "  the program options.\n");
  gmp_fprintf(stderr, "    -s <seed>   : Use <seed> as the random number seed. "
                      "Default: time()\n");
  gmp_fprintf(stderr, "    -b <bits>   : Public modulus n must have at least "
                      "<bits> bits. Default: 1024\n");
  gmp_fprintf(stderr, "    -i <iters>  : Run <iters> Miller-Rabin iterations "
                      "for primality testing. Default: 50\n");
  gmp_fprintf(
      stderr,
      "    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n");
  gmp_fprintf(
      stderr,
      "    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n");
  gmp_fprintf(stderr, "    -v          : Enable verbose output.\n");
  gmp_fprintf(stderr,
              "    -h          : Display program synopsis and usage.\n");
}
