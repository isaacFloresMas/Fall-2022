# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
// clang-format off
# include <gmp.h>
# include "numtheory.h"
# include "randstate.h"
// clang-format on

extern gmp_randstate_t state;

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits,
                  uint64_t iters) {
  uint64_t p_bits, q_bits;
  mpz_t totient_n, lambda_n, totient_p, totient_q, gcd_tptq, coprime;
  mpz_inits(totient_n, lambda_n, totient_p, totient_q, gcd_tptq, coprime, NULL);
  while (mpz_sizeinbase(n, 2) < nbits) {     // while bits in n < nbits
    p_bits = (random() % ((2 * nbits) / 4) + ((nbits / 4)));
    q_bits = nbits - p_bits;       	     // give b random bits in range
    make_prime(p, p_bits, iters);  	     // [nbits/4,(3×nbits)/4), give the rest to q
    make_prime(q, q_bits, iters);
    mpz_mul(n, p, q); 			     // n = p*q
  }
  mpz_sub_ui(totient_p, p, 1);               // φ(p) = p-1
  mpz_sub_ui(totient_q, q, 1);               // φ(q) = q-1
  mpz_mul(totient_n, totient_p, totient_q);  // φ(n) = φ(p)*φ(q)
  gcd(gcd_tptq, totient_p, totient_q);       // gcd(p-1, q-1)
  mpz_fdiv_q(lambda_n, totient_n, gcd_tptq); // λ(n) = φ(n) / gcd(p-1, q-1)
  while (mpz_cmp_ui(coprime, 1) != 0) {      // while e and λ(n) aren't coprime
    mpz_urandomb(e, state, nbits);
    gcd(coprime, e, lambda_n); 		     // coprime means gcd(e, λ(n)) = 1
  }
  mpz_clears(totient_n, lambda_n, totient_p, totient_q, gcd_tptq, coprime,
             NULL);
  return;
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
  gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
  gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
  mpz_t totient_n, totient_p, totient_q, gcd_pq, lambda_n;
  mpz_inits(totient_n, totient_p, totient_q, gcd_pq, lambda_n, NULL);
  mpz_sub_ui(totient_p, p, 1);              // φ(p) = p-1
  mpz_sub_ui(totient_q, q, 1);              // φ(q) = q-1
  mpz_mul(totient_n, totient_p, totient_q); // φ(n) = φ(p)*φ(q)
  gcd(gcd_pq, totient_p, totient_q);        // gcd(p-1, q-1)
  mpz_fdiv_q(lambda_n, totient_n, gcd_pq);  // λ(n) = φ(n) / gcd(p-1, q-1)
  mod_inverse(d, e, lambda_n);              // private key d = e % λ(n)
  mpz_clears(totient_n, totient_p, totient_q, gcd_pq, lambda_n, NULL);
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
  gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
  gmp_fscanf(pvfile, "%Zx\n%Zx\n", n, d);
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
  pow_mod(c, m, e, n); // E(m) = c = m^e (mod n)
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
  int k = ((mpz_sizeinbase(n, 2) - 1) / 8);  // k = ⌊(log2(n)−1)/8⌋
  mpz_t m, encrypted_m;
  mpz_inits(m, encrypted_m, NULL);
  int j;
  uint8_t *block;
  block =
      (uint8_t *)calloc(k, sizeof(uint8_t)); // allocate k bytes into block and
  *(block + 0) = 0xff;                       // pad the first byte with 0xff
  uint8_t *buffer;
  buffer = (uint8_t *)calloc(k, sizeof(uint8_t));

  while ((j = fread(buffer, sizeof(uint8_t), k - 1, infile)) !=0) { // while there are still unread bytes
    for (int index = 1; index <= j; index += 1) { // store j bytes into the block starting at first byte
      block[index] = buffer[index - 1];
    }
  if (j == k - 1) 				  // if j == k-1 import k bytes from block into m
    mpz_import(m, k, 1, sizeof(uint8_t), 1, 0, block);
  else 						  // if j != k-1 import j+1 bytes from block into m
    mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, block);
  rsa_encrypt(encrypted_m, m, e, n); 		  // encrypt message m
  gmp_fprintf(outfile, "%Zx\n", encrypted_m);     // print the encrypted message as a hexstring into
}						  // outfile
  mpz_clears(m, encrypted_m, NULL);
  free(block);
  free(buffer);
  block = NULL;
  buffer = NULL;
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
  pow_mod(m, c, d, n); // D(c) = m = c^d (mod n)
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
  int k = ((mpz_sizeinbase(n, 2) - 1) / 8);  // k = ⌊(log2(n)−1)/8⌋
  mpz_t c, m;
  mpz_inits(c, m, NULL);
  size_t j;
  uint8_t *blocked;
  blocked =
      (uint8_t *)calloc(k, sizeof(uint8_t)); // allocate k bytes into block
  while (gmp_fscanf(infile, "%Zx\n", c) != EOF) { // while there are still unread bytes
    rsa_decrypt(m, c, d, n); 		     // decrypt the read hexstring into m
    mpz_export(blocked, &j, 1, sizeof(uint8_t), 1, 0, m);
    					     // export m to the block and store the bytes converted in j
    fwrite(blocked + 1, sizeof(uint8_t), j - 1, outfile); 
    }					     // write j-1 bytes into the outfile starting from index 1
                			     // as to ignore the padding
  mpz_clears(c, m, NULL);
  free(blocked);
  blocked = NULL;
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
  pow_mod(s, m, d, n); // S(m) = s = m^d (mod n)
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
  mpz_t t;
  mpz_init(t);
  pow_mod(t, s, e, n);      // t = V(s) = s^e (mod n).
  if (mpz_cmp(t, m) == 0) { // if t == expected message m, return true
    mpz_clear(t);
    return true;
  } else { 		    // if t != expected message m, return false
    mpz_clear(t);
    return false;
  }
}
