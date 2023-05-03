#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// clang-format off
#include <gmp.h>
#include "randstate.h"
// clang-format on

extern gmp_randstate_t state;

void gcd(mpz_t d, mpz_t a, mpz_t b) {
  mpz_t temp, a_holder, b_holder;
  mpz_init(temp);
  mpz_init_set(a_holder, a);       // set temp a = a
  mpz_init_set(b_holder, b);       // set temb b = b
  while (mpz_sgn(b_holder) != 0) { // while b != 0
    mpz_set(temp, b_holder);
    mpz_mod(a_holder, a_holder, b_holder); // temp a = temp a % temp b
    mpz_set(b_holder, a_holder);
    mpz_set(a_holder, temp);
  }
  mpz_set(d, a_holder); 	   // d = gcd(a,b) = temp a
  mpz_clears(temp, a_holder, b_holder, NULL);
  return;
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
  mpz_t r, r_p, t, t_p, q, temp, holder;
  mpz_init_set(r, n);
  mpz_init_set(r_p, a); 	// (r,r′) ← (n,a)
  mpz_init(t);
  mpz_init_set_d(t_p, 1); 	// (t,t′) ← (0,1)
  mpz_inits(q, temp, holder, NULL);
  while (mpz_sgn(r_p) != 0) { 	// while r' != 0
    mpz_fdiv_q(q, r, r_p);    	// q ← ⌊r/r′⌋

    mpz_set(temp, r);           // temp = r
    mpz_set(r, r_p);            // r = r'
    mpz_mul(holder, q, r_p);    // holder = q * r'
    mpz_sub(r_p, temp, holder); // r′ = r − q * r′

    mpz_set(temp, t);           // temp = t
    mpz_set(t, t_p);            // t = t'
    mpz_mul(holder, q, t_p);    // holder = q * t'
    mpz_sub(t_p, temp, holder); // t' = t - q * t'
  }
  if (mpz_cmp_d(r, 1) > 0) {    // if r > 1
    mpz_set_d(o, 0);            // no mod inverse so o = 0
    mpz_clears(r, r_p, t, t_p, q, temp, holder, NULL);
    return;
  }
  if (mpz_sgn(t) == -1) { 	// if t < 0
    mpz_add(t, t, n);
  }
  mpz_set(o, t); 		// mod inverse o = t
  mpz_clears(r, r_p, t, t_p, q, temp, holder, NULL);
  return;
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
  mpz_t v, p, d_holder;
  mpz_init_set_d(v, 1);             	  // v = 1
  mpz_init_set(d_holder, d);        	  // temp d = d
  mpz_init_set(p, a);               	  // p = a
  while (mpz_sgn(d_holder) == 1) {  	  // while d > 0
    if (mpz_odd_p(d_holder) != 0) { 	  // if d is odd
      mpz_mul(v, v, p);
      mpz_mod(v, v, n); 		  // v = (v * p) mod n
    }
    mpz_mul(p, p, p);                     // p = (p * p) mod n
    mpz_mod(p, p, n);                     // p = (p * p) mod n
    mpz_fdiv_q_ui(d_holder, d_holder, 2); // d = ⌊d/2⌋
  }
  mpz_set(o, v); 			  // power mod o = v
  mpz_clears(v, p, d_holder, NULL);
  return;
}

// Miller-Rabin primality test
bool is_prime(mpz_t n, uint64_t iters) {
  mpz_t s, r, n_minus, a, j, i, y, y_holder, two;
  mpz_init_set(n_minus, n);
  mpz_sub_ui(n_minus, n_minus, 1); 	// n_minus == n - 1
  mpz_inits(s, a, y, j, y_holder, NULL);
  mpz_init_set_d(two, 2);

  if (mpz_cmp_ui(n, 1) == 0 || mpz_cmp_ui(n, 3) == 0) {
    return true;
  }
  if (mpz_cmp_ui(n, 2) == 0) {
    return false;
  }

  for (mpz_init_set(r, n_minus); mpz_odd_p(r) == 0;
       mpz_add_ui(s, s, 1)) { 		// r = n - 1 when s is 0, keep dividing
    mpz_fdiv_q_ui(r, r, 2);  	        // r by 2 and add 1 to s until r is odd
  }
  for (mpz_init_set_d(i, 1); mpz_cmp_d(i, iters) < 0;
       mpz_add_ui(i, i, 1)) { 		// for i = 1 to iters
    for (mpz_urandomm(a, state, n_minus);
         mpz_cmp_d(a, 2) < 0; 		// calculate random a∈ {2,3,...,n−2}
         mpz_urandomm(a, state, n_minus)) {}
    pow_mod(y, a, r, n); 		// y = POWER-MOD(a,r,n)
    if (mpz_cmp_d(y, 1) != 0 && mpz_cmp(y, n_minus) != 0) {     
      mpz_set_d(j, 1);			// if y != 1 and y != n-1
      while (mpz_cmp(j, s) < 0 &&
             mpz_cmp(y, n_minus) != 0) {// while j <= s-1 and y != n-1
        mpz_set(y_holder, y);
        pow_mod(y, y_holder, two, n);   // y = POWER-MOD(y,2,n)
        if (mpz_cmp_d(y, 1) == 0) {  	// if y == 1, return false
          mpz_clears(s, r, n_minus, a, j, i, y, y_holder, two, NULL);
          return false;
        }
        mpz_add_ui(j, j, 1); 		// j += 1
      }
      if (mpz_cmp(y, n_minus) != 0) { 	// if y != n-1, return false
        mpz_clears(s, r, n_minus, a, j, i, y, y_holder, two, NULL);
        return false;
      }
    }
  }
  mpz_clears(s, r, n_minus, a, j, i, y, y_holder, two, NULL);
  return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
  mpz_t prime_num;
  mpz_init(prime_num);
  for (mpz_rrandomb(prime_num, state, bits); is_prime(prime_num, iters) == 0 ||
       mpz_sizeinbase(prime_num, 2) < bits; mpz_rrandomb(prime_num, state, bits)) {}                     
  mpz_set(p, prime_num);     		// make a random number about bits long, keep looping
  mpz_clear(prime_num);			// if the number isn't prime or if it is less than bits long
  return;
}
