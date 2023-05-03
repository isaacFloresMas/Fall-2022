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

extern uint64_t bytes_written;

int main(int argc, char **argv) {
  int opt = 0;
  // Default values for options.
  char *input = "stdin";
  char *output = "stdout";
  bool stats = false;
  // Parsing user options.
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
  int in = open(input, O_RDONLY);
  uint64_t *freq = (uint64_t *)calloc(ALPHABET, sizeof(uint64_t));
  uint8_t user[BLOCK];
  int just_read;
  // Read through infile until everything has been read.
  // Add every character's occurrence to a histogram.
  while ((just_read = read_bytes(in, user, BLOCK)) != 0) {
    for (int j = 0; j < just_read; j++) {
      freq[user[j]] += 1;
    }
  }
  // If the first and second index of the histogram are 0.
  // Set them to 1.
  if (freq[0] == 0)
    freq[0] = 1;
  if (freq[1] == 0)
    freq[1] = 1;
  // Build the tree with the histogram.
  Node *root = build_tree(freq);
  // Build a code table starting from the Huffman tree's root.
  Code code_table[ALPHABET];
  build_codes(root, code_table);

  // Gathering header data.
  Header data;
  data.magic = MAGIC;
  struct stat perm;
  // Get infile's statistics
  fstat(in, &perm);
  data.permissions = perm.st_mode;
  data.file_size = perm.st_size;
  uint16_t unique = 0;
  // Get the unique characters from the histogram.
  for (int i = 0; i < ALPHABET; i++) {
    if (freq[i] > 0)
      unique += 1;
  }
  // Calculate the tree size with the unique characters.
  data.tree_size = (3 * unique) - 1;
  // Cast the header data to a byte array.
  uint8_t *header_p = (uint8_t *)&data;
  int out;
  // Open stdout if users wants that or the given output location.
  if (strcmp(output, "stdout") == 0)
    out = STDOUT_FILENO;
  else
    out = open(output, O_WRONLY | O_CREAT);
  // Set outfile's permissions to the infile's permissions.
  fchmod(out, data.permissions);
  // Write the header data to the outfile.
  write_bytes(out, header_p, sizeof(Header));
  // End of gathering header data.
  
  // Dump the Huffman tree starting from the root.
  dump_tree(out, root);
  // Reset the infile so it can be read from the start.
  lseek(in, 0, SEEK_SET);
  uint8_t in_file[BLOCK];
  // Read the infile so that each characters code can be written.
  while ((just_read = read_bytes(in, in_file, BLOCK)) != 0) {
    for (int j = 0; j < just_read; j++) {
      write_code(out, &code_table[in_file[j]]);
    }
  }
  // Flush any remaining code.
  flush_codes(out);
  // Print statistics if option was given.
  if (stats) {
    double saved = ((double)bytes_written / (double)data.file_size);
    fprintf(stderr, "Uncompressed file size: %lu bytes\n", data.file_size);
    fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_written);
    fprintf(stderr, "Space saving: %.2f%%\n", (double)(100 * (1 - saved)));
  }
  // Closing files, freeing arrays, and deleting the Huffman tree.
  delete_tree(&root);
  close(out);
  close(in);
  free(freq);
  return 0;
}

// Help and usage messgae.
void print_help(void) {
  fprintf(stderr, "SYNOPSIS\n");
  fprintf(stderr, "  A Huffman encoder.\n");
  fprintf(stderr,
          "  Compresses a file using the Huffman coding algorithm.\n\n");
  fprintf(stderr, "USAGE\n");
  fprintf(stderr, "  ./encode [-h] [-i infile] [-o outfile]\n\n");
  fprintf(stderr, "OPTIONS\n");
  fprintf(stderr, "  -h             Program usage and help.\n");
  fprintf(stderr, "  -v             Print compression statistics.\n");
  fprintf(stderr, "  -i infile      Input file to compress.\n");
  fprintf(stderr, "  -o outfile     Output of compressed data.\n");
}
