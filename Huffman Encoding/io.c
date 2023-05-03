#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "code.h"
#include "defines.h"

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
// Global buffer and indices.
static uint8_t bit_buffer[BLOCK];
static uint32_t bit_index = 0;
static uint8_t wr_buffer[BLOCK];
static uint32_t wr_index = 0;
static int size;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
  int read_bytes = 0;
  int rb = 0;
  // While nbytes hasn't been read.
  while (read_bytes < nbytes) {
    // Read nbytes - bytes already read into the buffer passed in.
    rb = read(infile, buf + rb, nbytes - rb);
    // If 0 bytes were read, break.
    if (rb == 0)
      break;
    // Add the bytes just read to the total bytes read.
    read_bytes += rb;
  }
  // Add the total bytes read to the external bytes read.
  bytes_read += read_bytes;
  // Return the total bytes read.
  return read_bytes;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
  int wr_bytes = 0;
  int wb = 0;
  // While nbytes hasn't been written.
  while (wr_bytes < nbytes) {
    // Write nbytes - bytes already written into the buffer passed in.
    wb = write(outfile, buf + wb, nbytes - wb);
    // If 0 bytes were written, break.
    if (wb == 0)
      break;
    // Add the bytes just written to the total bytes written.
    wr_bytes += wb;
  }
  // Add the total bytes written to the external bytes written.
  bytes_written += wr_bytes;
  // Return the total bytes written.
  return wr_bytes;
}

bool read_bit(int infile, uint8_t *bit) {
  // If the bit buffer index is 0, read BLOCK bytes into the buffer.
  if (bit_index == 0) { 
    // If 0 bytes were read, return false.
    if ((size = read_bytes(infile, bit_buffer, BLOCK)) == 0) {
      bit_index = 0;
      return false;
    }
  }
  // If the bytes read isn't BLOCK and the bit buffer index has reached the
  // end, return false.
  if (size != BLOCK && (int)bit_index == size * 8) {
    bit_index = 0;
    return false;
  }
  // Get the bit at the current bit buffers index and if it is not 0, set
  // the bit pointer passed in to 1.
  if ((*bit = bit_buffer[bit_index / 8] & ((uint8_t)(1) << (bit_index % 8))) !=
      0)
    *bit = 1;
  // Else, set the bit pointer passed in to 0.
  else
    *bit = 0;
  // If the bit buffer has been fully read, reset the bit buffer index.
  if (bit_index == BLOCK * 8 - 1)
    bit_index = 0;
  // Else, increment the bit buffer index.
  else
    bit_index += 1;
  return true;
}

void write_code(int outfile, Code *c) {
  // Get the code size.
  uint32_t size = code_size(c);
  uint32_t curr = 0;
  // For each accessible bit in the code.
  while (curr < size) {
    // If the current bit is a 1, set the current write buffer's index to 1.
    if (code_get_bit(c, curr)) {
      wr_buffer[wr_index / 8] |= ((uint8_t)(1) << (wr_index % 8));
    // Else, clear the current write buffer's index.
    } else {
      wr_buffer[wr_index / 8] &= ~((uint8_t)(1) << (wr_index % 8));
    }
    // Increment the write buffer's index and the current code bit.
    wr_index += 1;
    curr += 1;
    // If the write buffer is filled, write its contents to outfile
    // and reset the write buffer's index to 0.
    if (wr_index == BLOCK * 8) {
      write_bytes(outfile, wr_buffer, BLOCK);
      wr_index = 0;
    }
  }
}

void flush_codes(int outfile) {
  // Clear all bits to the right of where the write buffer left off.
  for (uint64_t i = wr_index; i < BLOCK * 8; i++) {
    wr_buffer[i / 8] &= ~((uint8_t)(1) << (i % 8));
  }
  // Divide the write buffer's index by 8 and round up.
  wr_index = (wr_index % 8 == 0) ? wr_index / 8 : wr_index / 8 + 1;
  // Write any remaining code to the outfile.
  write_bytes(outfile, wr_buffer, wr_index);
  wr_index = 0;
}
