#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"

uint8_t one = 1;

typedef struct {
  uint32_t top;
  uint8_t bits[MAX_CODE_SIZE];
} Code;

Code code_init(void) {
  Code c;
  (&c)->top = 0; // Set the code's top to 0.
  // Set every byte in the bit array to 0.
  for (int i = 0; i < MAX_CODE_SIZE; i++) {
    (&c)->bits[i] = 0;
  }
  // Return the initialized code.
  return c;
}

// Returns the top of the code.
uint32_t code_size(Code *c) { return c->top; }

// Returns true if the top equals to 0.
bool code_empty(Code *c) {
  if (c->top != 0)
    return false;
  return true;
}

// Returns true if the top equals to ALPHABET.
bool code_full(Code *c) { return (c->top == ALPHABET); }

bool code_set_bit(Code *c, uint32_t i) {
  if (i >= ALPHABET) // if i is out of range, return false.
    return false;
  uint8_t loc = i / 8; // location of bit is i / 8
  uint8_t pos = i % 8; // bit position in the location is i % 8
  uint8_t set_bit = one << pos; // Left shift one position times.
  // Byte at location equals to itself bitwise or the previously made number.
  c->bits[loc] = c->bits[loc] | set_bit;
  return true;
}

bool code_clr_bit(Code *c, uint32_t i) {
  if (i >= ALPHABET) // if i is out of range, return false.
    return false;
  uint8_t loc = i / 8; // location of bit is i / 8
  uint8_t pos = i % 8; // bit position in the location is i % 8
  uint8_t set_bit = one << pos; // Left shift one position times.
  // Byte at location equals to itself bitwise and the inverse of
  // the previously made number.
  c->bits[loc] = c->bits[loc] & ~(set_bit);
  return true;
}

bool code_get_bit(Code *c, uint32_t i) {
  if (i >= ALPHABET) // if i is out of range, return false.
    return false;
  uint8_t loc = i / 8; // location of bit is i / 8
  uint8_t pos = i % 8; // bit position in the location is i % 8
  uint8_t set_bit = one << pos; // Left shift one position times.
  // Get bit equals to the byte at location bitwise and the
  // previously made number.
  uint8_t get_bit = c->bits[loc] & set_bit; 
  // If get bit is not 0, return true, else return false.
  if (get_bit != 0)
    return true;
  return false;
}

bool code_push_bit(Code *c, uint8_t bit) {
  // If code is full, return false.
  if (code_full(c))
    return false;
  // If the bit passed in is 1, set the top bit.
  if (bit == 1)
    code_set_bit(c, c->top);
  // Else, clear the top bit.
  else
    code_clr_bit(c, c->top);
  // Increment the top size.
  c->top += 1;
  return true;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
  // If the code is empty, return false.
  if (code_empty(c))
    return false;
  // Decrement the top size if it is not 0.
  if (c->top != 0)
    c->top -= 1;
  // If the top bit is set, bit pointer passed in = 1.
  if (code_get_bit(c, c->top)) 
    *bit = 1;
  // If the top bit is set, bit pointer passed in = 0.
  else
    *bit = 0;
  // Clear the popped bit.
  code_clr_bit(c, c->top);
  return true;
}

// Debug function for code.
void code_print(Code *c) {
  for (uint32_t i = 0; i < MAX_CODE_SIZE; i++) {
    printf("%hhu ", c->bits[i]);
  }
  printf("\n");
}
