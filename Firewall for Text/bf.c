#include "bv.h"
#include "city.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define N_HASHES 5

typedef struct BloomFilter BloomFilter;

struct BloomFilter {
  uint64_t salts[N_HASHES]; // Salts for five hash functions
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_bits_examined;
  BitVector *filter;
};

static uint64_t default_salts[] = {0x5adf08ae86d36f21, 0x419d292ea2ffd49e,
                                   0x50d8bb08de3818df, 0x272347aea4045dd5,
                                   0x7c8e16f768811a21};

BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->n_keys = bf->n_hits = 0;
    bf->n_misses = bf->n_bits_examined = 0;
    for (int i = 0; i < N_HASHES; i++) {
      bf->salts[i] = default_salts[i];
    }
    bf->filter = bv_create(size);
    if (bf->filter == NULL) {
      free(bf);
      bf = NULL;
    }
  }
  return bf;
}

void bf_delete(BloomFilter **bf) {
  bv_delete(&(*bf)->filter); // frees the underlying blt vector
  free(*bf); // frees the bloom filter
  *bf = NULL;
}

// returns length of the underlying bit bector
uint32_t bf_size(BloomFilter *bf) {
  uint32_t size = bv_length(bf->filter);
  return size;
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
  uint64_t index;
  bf->n_keys += 1; // one key was added
  for (int i = 0; i < 5; i += 1) { // for each five salts
    index = hash(bf->salts[i], oldspeak);
    index = index % (bf_size(bf)); // hash the oldspeak and modulo it by the size
    if (bv_get_bit(bf->filter, index) == 0) { // if not set yet, set the bit
      bv_set_bit(bf->filter, index);
    }
    // else don't set the bit
  }
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
  uint64_t index;
  uint8_t is_set;
  for (int i = 0; i < 5; i += 1) { // for each five salts
    index = hash(bf->salts[i], oldspeak);
    index = index % (bf_size(bf)); // hash the oldspeak and modulo it by the size
    is_set = bv_get_bit(bf->filter, index); // check if the bit is set
    bf->n_bits_examined += 1; // a bit was examined
    if (is_set == 0) { // if it was not set, return false and add a miss
      bf->n_misses += 1;
      return false;
    }
  }
  bf->n_hits += 1; // all five bits were set so there was a hit
  return true;
}

uint32_t bf_count(BloomFilter *bf) {
  uint32_t count = 0;
  uint32_t length = bf_size(bf);
  for (uint32_t bit = 0; bit < length; bit += 1) { // for each available bit
    if (bv_get_bit(bf->filter, bit) == 1) { // if the bit is set, add 1 to count
      count += 1;
    }
  }
  return count;
}

// prints the underlying bit vector
void bf_print(BloomFilter *bf) { bv_print(bf->filter); } 

// sets the paramters to the bloom filter statistics
void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
  *nk = bf->n_keys;
  *nh = bf->n_hits;
  *nm = bf->n_misses;
  *ne = bf->n_bits_examined;
}
