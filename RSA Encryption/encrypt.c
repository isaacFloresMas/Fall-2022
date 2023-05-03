#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// clang-format off
#include <gmp.h>
#include "numtheory.h"
#include "rsa.h"
// clang-format on

int main(int argc, char **argv) {
  int opt = 0;
  char *pubfile;
  char *encrypt_in;
  char *encrypt_out;
  pubfile = "rsa.pub";
  encrypt_in = "stdin";
  encrypt_out = "stdout";
  int verbose = 0;
  while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
    switch (opt) {
    case 'i': {
      encrypt_in = optarg;
      break;
    }
    case 'o': {
      encrypt_out = optarg;
      break;
    }
    case 'n': {
      pubfile = optarg;
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
  FILE *fpublic;
  if ((fpublic = fopen(pubfile, "r")) == NULL) { // open the public key file. Print error if problem
    fprintf(stderr, "./encrypt: couldn't open %s to read public key.\n", // occurred
            pubfile);
    return 1;
  }

  FILE *e_in;
  if (strcmp(encrypt_in, "stdin") == 0) { // if standard input was given, read user input and write
    char input[1]; 			  // it to a temporary file
    e_in = fopen("entemp_in.txt", "w+");
    while (scanf("%c", input) != EOF) {
      fprintf(e_in, "%c", input[0]);
    }
    rewind(e_in);
  } else if ((e_in = fopen(encrypt_in, "r")) == NULL) { // if standard input wasn't given, open the
							// desired file.
    fprintf(stderr, "./encrypt: Couldn't open %s to read plaintext: No such file or directory\n",
            encrypt_in); 				// print an error if there was a problem
    return 1;
  }
  FILE *e_out;
  if (strcmp(encrypt_out, "stdout") == 0) {// if standard output was given, open a temporary
    e_out = fopen("entemp_out.txt", "w+"); // file to store the encryption output
  } else {                                 // else create the desired output file
    e_out = fopen(encrypt_out, "w+");
  }
  char user[100];
  mpz_t pub_mod, pub_exp, sign, expected_message;
  mpz_inits(pub_mod, pub_exp, sign, expected_message, NULL);
  rsa_read_pub(pub_mod, pub_exp, sign, user, fpublic); // read the public key data
  if (verbose) {         			       // if verbose is wanted, print the verbose output
    void verbose_print(char *username, mpz_t signature, mpz_t n, mpz_t e);
    verbose_print(user, sign, pub_mod, pub_exp);
  }
  mpz_set_str(expected_message, user, 62); // convert the read username to an mpz_t and verify it
  if (!(rsa_verify(expected_message, sign, pub_exp, pub_mod))) { // if signature couldn't be verified,
    fprintf(stderr, "./encrypt: Couldn't verify user signature - exiting!\n"); //print and error & exit
    return 1;
  }
  rsa_encrypt_file(e_in, e_out, pub_mod, pub_exp); // encrypt the file
  if (strcmp(encrypt_out, "stdout") == 0) {        // if standard output is wanted
    mpz_t out;
    mpz_init(out);
    rewind(e_out);
    printf("\n");
    while (gmp_fscanf(e_out, "%Zx\n", out) != EOF) { // print the encrypted data out line for line
      gmp_printf("%Zx\n", out);
    }
    mpz_clear(out);
  }
  fclose(fpublic);
  fclose(e_in);
  fclose(e_out);
  mpz_clears(pub_mod, pub_exp, sign, expected_message, NULL);
  return 0;
}

void verbose_print(char *username, mpz_t signature, mpz_t n, mpz_t e) {
  gmp_fprintf(stderr, "username: %s\n", username);
  gmp_fprintf(stderr, "user signature (%d bits): %Zd\n",
              mpz_sizeinbase(signature, 2), signature);
  gmp_fprintf(stderr, "n - modulus (%d bits): %Zd\n", mpz_sizeinbase(n, 2), n);
  gmp_fprintf(stderr, "e - public exponent (%d bits): %Zd\n",
              mpz_sizeinbase(e, 2), e);
}

void print_help(void) {
  gmp_fprintf(stderr, "Usage: ./encrypt [options]\n");
  gmp_fprintf(stderr, "  ./encrypt encrypts an input file using the specified "
                      "public key file,\n");
  gmp_fprintf(stderr, "  writing the result to the specified output file.\n");
  gmp_fprintf(
      stderr,
      "    -i <infile> : Read input from <infile>. Default: standard input.\n");
  gmp_fprintf(stderr, "    -o <outfile>: Write output to <outfile>. Default: "
                      "standard output.\n");
  gmp_fprintf(
      stderr,
      "    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n");
  gmp_fprintf(stderr, "    -v          : Enable verbose output.\n");
  gmp_fprintf(stderr,
              "    -h          : Display program synopsis and usage.\n");
}
