#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

extern uint64_t bytes_read;

int main(int argc, char **argv) {
  int opt = 0;
  // Default user options.
  char *input = "stdin";
  char *output = "stdout";
  bool stats = false;
  // Parse user options.
  while ((opt = getopt(argc, argv, "hi:o:v")) != -1) {
    switch (opt) {
    case 'h': {
      void print_help(void);
      print_help();
      return 0;
    }
    case 'i': {
      input = optarg;
      break;
    }
    case 'o': {
      output = optarg;
      break;
    }
    case 'v': {
      stats = true;
      break;
    }
    // If invalid option was given, print help and exit.
    default: {
      void print_help(void);
      print_help();
      return 1;
    }
    }
  }
  int in;
  // If user wants standard input for the infile.
  if (strcmp(input, "stdin") == 0) {
    int jrc;
    // Create a temporary file that will be used to hold stdin data.
    uint8_t temp_buf[1];
    uint8_t temp_write[BLOCK];
    uint32_t twi = 0;
    int in_temp = STDIN_FILENO;
    in = open("in_temp.txt", O_CREAT | O_RDWR | O_TRUNC, 0755);
    // Read 1 byte at a time so that the program doesn't read stdin
    // faster than it can be written.
    while ((jrc = read_bytes(in_temp, temp_buf, 1)) != 0) {
      temp_write[twi] = temp_buf[0];
      twi += 1;
      // Slowly fill up a BLOCK sized buffer to be more efficient.
      // Write the buffer when it is filled.
      if (twi == BLOCK) {
        write_bytes(in, temp_write, BLOCK);
        twi = 0;
      }
    }
    // Flushes any remaining data in the buffer.
    if (twi != 0) {
      write_bytes(in, temp_write, twi);
    }
    // Reset the infile so that it can be read from the start.
    lseek(in, 0, SEEK_SET);
    bytes_read = 0;
  } 
  // Else, open the given infile location.
  else
    in = open(input, O_RDONLY);

  // Gathering header data
  Header data;
  // Cast the header data into a byte array.
  uint8_t *data_in = (uint8_t *)&data;
  // Read in the header data from the infile.
  read_bytes(in, data_in, sizeof(Header));
  // Get the infile's magic number.
  uint32_t in_magic = 0;
  in_magic |= (uint32_t)data_in[0] | (uint32_t)data_in[1] << 8 |
              (uint32_t)data_in[2] << 16 | (uint32_t)data_in[3] << 24;
  // If an incorrect magic number was read, given an error message and exit.
  if (in_magic != MAGIC) {
    fprintf(stderr, "Invalid magic number.\n");
    return 1;
  }
  // Getting the infiles permissions.
  uint16_t perm = 0;
  perm |= (uint16_t)data_in[4] | (uint16_t)data_in[5] << 8;
  // Opening stdout if default and the given location if given.
  int out;
  if (strcmp(output, "stdout") == 0)
    out = STDOUT_FILENO;
  else {
    out = open(output, O_WRONLY | O_CREAT);
  }
  // Putting the infile's permissions into the outfile.
  fchmod(out, perm);
  // Getting the tree size from infile.
  uint16_t size_tree = 0;
  size_tree |= (uint16_t)data_in[6] | (uint16_t)data_in[7] << 8;
  // Getting the original file size from infile.
  uint64_t size_file = 0;
  size_file |= (uint64_t)data_in[8] | (uint64_t)data_in[9] << 8 |
               (uint64_t)data_in[10] << 16 | (uint64_t)data_in[11] << 24 |
               (uint64_t)data_in[12] << 32 | (uint64_t)data_in[13] << 40 |
               (uint64_t)data_in[14] << 48 | (uint64_t)data_in[15] << 56;
  // Alocate a buffer for the Huffman tree.
  uint8_t *huff_tree = (uint8_t *)malloc(size_tree * sizeof(uint8_t));
  // Read the dumped tree data and rebuild the tree.
  read_bytes(in, huff_tree, size_tree);
  Node *root = rebuild_tree(size_tree, huff_tree);
  // Set the current node to the root node. 
  Node *curr = root;
  uint8_t decoded[BLOCK];
  uint32_t id = 0;
  uint32_t curr_size = 0;
  uint8_t bit;
  // Read the written code bit by bit.
  while (read_bit(in, &bit)) {
    // If the read bit was a 0 go to the current node's left child.
    // Else, go to it's right child.
    if (bit == 0) {
      curr = curr->left;
    } else {
      curr = curr->right;
    }
    // If the current node is a leaf, put it's symbol into a buffer,
    // increment the buffer's index and current outfile size, then
    // reset the current node to the root node.
    if (curr->left == NULL && curr->right == NULL) {
      decoded[id] = curr->symbol;
      id += 1;
      curr_size += 1;
      curr = root;
    }
    // If the buffer is filled up, write its contents to the outfile
    // and reset the buffer's index.
    if (id == BLOCK) {
      write_bytes(out, decoded, BLOCK);
      id = 0;
    }
    // If the original file size is reached, break.
    if (curr_size == size_file)
	break;
  }
  // Flush any remaining data from the buffer.
  if (id != 0) {
    write_bytes(out, decoded, id);
  }
  // Print statistics if the user wants them.
  if (stats) {
    double saved = ((double)bytes_read / (double)size_file);
    fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_read);
    fprintf(stderr, "Decompressed file size: %lu bytes\n", size_file);
    fprintf(stderr, "Space saving: %.2f%%\n", (double)(100 * (1 - saved)));
  }
  // Close all files, free any arrays, and delete the Huffman tree.
  free(huff_tree);
  delete_tree(&root);
  close(in);
  close(out);
}

// Print help and usage information.
void print_help(void) {
  fprintf(stderr, "SYNOPSIS\n");
  fprintf(stderr, "  A Huffman decoder.\n");
  fprintf(stderr,
          "  Decompresses a file using the Huffman coding algorithm.\n\n");
  fprintf(stderr, "USAGE\n");
  fprintf(stderr, "  ./decode [-h] [-i infile] [-o outfile]\n\n");
  fprintf(stderr, "OPTIONS\n");
  fprintf(stderr, "  -h             Program usage and help.\n");
  fprintf(stderr, "  -v             Print compression statistics.\n");
  fprintf(stderr, "  -i infile      Input file to decompress.\n");
  fprintf(stderr, "  -o outfile     Output of decompressed data.\n");
}
