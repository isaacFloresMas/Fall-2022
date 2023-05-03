#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// clang-format off
# include <gmp.h>
#include "numtheory.h"
#include "rsa.h"
// clang-format on

int main(int argc, char **argv) {
  int opt = 0;
  char *privfile;
  char *decrypt_in;
  char *decrypt_out;
  privfile = "rsa.priv";
  decrypt_in = "stdin";
  decrypt_out = "stdout";
  int verbose = 0;
  while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
    switch (opt) {
    case 'i': {
      decrypt_in = optarg;
      break;
    }
    case 'o': {
      decrypt_out = optarg;
      break;
    }
    case 'n': {
      privfile = optarg;
      break;
    }
    case 'v': {
      verbose = 1;
      break;
    }
    case 'h': {
      void print_help(void);
      print_help();
      return 0;
    }
    }
  }
  FILE *fprivate; 			    // open the private key file and print an error message
  if ((fprivate = fopen(privfile, "r")) == NULL) { // if there was a problem
    fprintf(stderr, "./decrypt: couldn't open %s to read private key.\n",
            privfile);
    return 1;
  }

  FILE *d_in;
  if (strcmp(decrypt_in, "stdin") == 0 ) {  // if the user wants standard input, create a temporary file
    mpz_t input; 			    // to store the users input
    mpz_init(input);
    d_in = fopen("detemp_in.txt", "w+");
    while (gmp_scanf("%Zx", input) != EOF) {// scan in the encrypted data and store it into the temporary
      gmp_fprintf(d_in, "%Zx\n", input);    // file
    }
    rewind(d_in);
    mpz_clear(input);
  } else if ((d_in = fopen(decrypt_in, "r")) == NULL) {
    fprintf(stderr, "decrypt: Couldn't open %s to read directory\n", decrypt_in);
    return 1;				    // open desired file if user doesn't want standard input
  }
  FILE *d_out; 				    // if the user wants standard output, create a temporary file
  if (strcmp(decrypt_out, "stdout") == 0) { // to store the decrypted text
    d_out = fopen("detemp_out.txt", "w+");
  } else { 				    // else create the desired file to store the output
    d_out = fopen(decrypt_out, "w+");
  }

  mpz_t priv_key, pub_mod;
  mpz_inits(priv_key, pub_mod, NULL);
  rsa_read_priv(pub_mod, priv_key, fprivate); // read the private key data
  if (verbose) { 			      // if verbose output is wanted, print the verbose data
    void verbose_print(mpz_t n, mpz_t d);
    verbose_print(pub_mod, priv_key);
  }
  rsa_decrypt_file(d_in, d_out, pub_mod, priv_key); // decrypt the encrypted data
  if (strcmp(decrypt_out, "stdout") == 0) {   // if the user wants standard output, print the decrypted
    char output[1]; 			      // text
    rewind(d_out);
    while (fscanf(d_out, "%c", output) != EOF) {
      printf("%c", output[0]);
    }
  }

  mpz_clears(priv_key, pub_mod, NULL);
  fclose(fprivate);
  fclose(d_in);
  fclose(d_out);
  return 0;
}

void verbose_print(mpz_t n, mpz_t d) {
  gmp_fprintf(stderr, "n - modulus (%d bits): %Zd\n", mpz_sizeinbase(n, 2), n);
  gmp_fprintf(stderr, "d - private exponent (%d bits): %Zd\n",
              mpz_sizeinbase(d, 2), d);
}

void print_help(void) {
  gmp_fprintf(stderr, "Usage: ./decrypt [options]\n");
  gmp_fprintf(stderr, "  ./decrypt decrypts an input file using the specified "
                      "private key file,\n");
  gmp_fprintf(stderr, "  writing the result to the specified output file.\n");
  gmp_fprintf(
      stderr,
      "    -i <infile> : Read input from <infile>. Default: standard input.\n");
  gmp_fprintf(stderr, "    -o <outfile>: Write output to <outfile>. Default: "
                      "standard output.\n");
  gmp_fprintf(
      stderr,
      "    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n");
  gmp_fprintf(stderr, "    -v          : Enable verbose output.\n");
  gmp_fprintf(stderr,
              "    -h          : Display program synopsis and usage.\n");
}
