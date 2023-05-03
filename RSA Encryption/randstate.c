#include <gmp.h>
#include <stdint.h>
#include <stdlib.h>

gmp_randstate_t state;

void randstate_init(uint64_t seed) {
  srandom(seed);                // set the random seed
  gmp_randinit_mt(state);       // initialize state for Mersenne-Twister
  gmp_randseed_ui(state, seed); // initialize the seed value into state
}

void randstate_clear(void) { 
  gmp_randclear(state);         // free memory occupied by the state
}
