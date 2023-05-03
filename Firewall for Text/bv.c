#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t one = 1;

typedef struct BitVector BitVector;

struct BitVector {
  uint32_t length;
  uint64_t *vector;
};

BitVector *bv_create(uint32_t length) {
  // allocate memory for the bit vector
  BitVector *bv = (BitVector *)malloc(sizeof(BitVector));
  bv->length = length;
  // set the size to the given length / 64 rounding up if their is no remainder
  uint64_t size = (length % 64 == 0) ? length / 64 : (length / 64) + 1;
  // allocate size amount of uint64_t to the vector
  bv->vector = (uint64_t *)calloc(size, sizeof(uint64_t));
  if (bv->vector == NULL) { // if there was an error when allocating
    free(bv->vector); // free the memory for the vector and bit vector
    free(bv);
    bv = NULL;
    printf("Couldn't allocate %d bits to the bit vector", length);
    return NULL;
  }
  // return the new bit vector
  return bv;
}

void bv_delete(BitVector **bv) {
  free((*bv)->vector); // free the vector
  free(*bv); // free the bit vector
  *bv = NULL;
}

// returns the length of the bit vector
uint32_t bv_length(BitVector *bv) { return bv->length; }

void bv_set_bit(BitVector *bv, uint32_t i) {
  uint64_t byte = i / 64; // location of bit is i / 64
  uint64_t position = i % 64; // bit position in the location is i % 64
  uint64_t set_bit = one << position; // left shift 1 position times
  // bitwise or the left shifted bit with the byte to set the ith bit
  bv->vector[byte] = bv->vector[byte] | set_bit;
}

void bv_clr_bit(BitVector *bv, uint32_t i) {
  uint64_t byte = i / 64; // location of bit is i / 64
  uint64_t position = i % 64; // bit position in the location is i % 64
  uint64_t clear_bit = one << position; // left shift 1 position times
  // bitwise and the inversed left shifted bit with the byte to clear the ith bit
  bv->vector[byte] = bv->vector[byte] & ~(clear_bit);
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
  uint64_t byte = i / 64; // location of bit is i / 64
  uint64_t position = i % 64; // bit position in the location is i % 64
  uint64_t bitwise = one << position; // left shift 1 position times
  // bitwise and the left shifted bit with the byte to get the ith bit
  uint64_t get_bit = bv->vector[byte] & bitwise;
  uint8_t i_bit = 0;
  if (get_bit != 0) { // if the ith bit is not 0, the bit is set
    i_bit = 1; 
  }
  return i_bit;
}

// prints every uint64_t in the bit vector
void bv_print(BitVector *bv) {
  uint32_t length_t = bv->length;
  uint32_t size = (length_t % 64 == 0) ? length_t / 64 : (length_t / 64) + 1;
  for (uint32_t index = 0; index < size; index += 1) {
    printf("%lu ", bv->vector[index]);
  }
  printf("\n");
}
