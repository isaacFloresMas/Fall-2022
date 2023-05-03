#include "bf.h"
#include "city.h"
#include "ht.h"
#include "ll.h"
#include "messages.h"
#include "parser.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_HASHES 5

extern uint32_t seeks; // Number of seeks performed.
extern uint32_t links; // Number of links traversed.

int main(int argc, char **argv) {
  int opt = 0;
  uint32_t hash_size = 10000; // default: 10000
  uint32_t bloom_size = 524288; // default: 2^19
  bool statistics = false; // default: false
  bool move2front = false; // default: false
  while ((opt = getopt(argc, argv, "ht:f:ms")) != -1) {
    switch (opt) {
    case 't': {
      if ((hash_size = atoi(optarg)) < 1) { // if invalid size, print error message
        printf("./banhammer: Invalid hash table size.\n");
        return 1;
      } 
      else {
        break;
      }
    }
    case 'f': {
      if ((bloom_size = atoi(optarg)) < 1) { // if invalid size, print error message
        printf("./banhammer: Invalid bloom filter table size.\n");
        return 1;
      } 
      else {
        break;
      }
    }
    case 'm': { // if user wants move-to-front enabled
      move2front = true;
      break;
    }
    case 's': { // if user wants statistics
      statistics = true;
      break;
    }
    case 'h': { // if help is wanted
      void print_help(void);
      print_help();
      return 0;
    }
    }
  }
  // make the hash table, bloom filter, and parser for user input
  HashTable *hash = ht_create(hash_size, move2front);
  BloomFilter *boyf = bf_create(bloom_size);
  Parser *parse = parser_create(stdin);
  // open the badspeak and newspeak files for reading
  FILE *bad_s = fopen("badspeak.txt", "r");
  FILE *new_s = fopen("newspeak.txt", "r");
  char old_buffer[MAX_PARSER_LINE_LENGTH];
  char new_buffer[MAX_PARSER_LINE_LENGTH];
  // make the parsers to read the badspeak and newspeak files
  Parser *bad_parse = parser_create(bad_s);
  Parser *old_parse = parser_create(new_s);
  // while the badspeak file has not finished
  while (next_word(bad_parse, old_buffer)) { // read in the badspeak word
    bf_insert(boyf, old_buffer); // insert the badspeak into the bloom filter
    ht_insert(hash, old_buffer, NULL); // insert the badspeak into the hash table
  }
  // while the newspeak file has not finished
  while (next_word(old_parse, old_buffer)) { // read in the oldspeak word
    next_word(old_parse, new_buffer); // read in the newspeak word
    bf_insert(boyf, old_buffer); // insert the oldspeak into the bloom filter
    ht_insert(hash, old_buffer, new_buffer); // insert the oldspeak & newspeak into the hash table
  }
  // close the files and delete the parsers
  fclose(bad_s);
  fclose(new_s);
  parser_delete(&bad_parse);
  parser_delete(&old_parse);
  char c_word[MAX_PARSER_LINE_LENGTH];
  // create two linked lists for the badspeak and oldspeak the user uses
  LinkedList *used_bad_words = ll_create(false);
  LinkedList *used_old_words = ll_create(false);
  // while the user has finsihed inputting text
  while (next_word(parse, c_word)) { // get the next available word
    // if the word is in the bloom filter
    if (bf_probe(boyf, c_word)) {
      // find the node the word is inside the hash table
      Node *search_word = ht_lookup(hash, c_word);
      // if the word is badspeak
      if (search_word != NULL && search_word->newspeak == NULL) {
	// insert the badspeak into the linked list of badspeak words used
        ll_insert(used_bad_words, search_word->oldspeak, search_word->newspeak);
      }
      // if the word is oldspeak 
      else if (search_word != NULL && search_word->newspeak != NULL) {
	// insert the oldspeak with its newspeak into the linked list of oldspeak words used
        ll_insert(used_old_words, search_word->oldspeak, search_word->newspeak);
      }
    }
  }
  // if the user wants statistics to print
  if (statistics) {
    void print_stats(uint32_t ht_k, uint32_t ht_h, uint32_t ht_m, uint32_t ht_p,
                     uint32_t bf_k, uint32_t bf_h, uint32_t bf_m,
                     uint32_t bf_be, uint32_t count, uint32_t size);
    uint32_t n_keys, n_hits, n_misses, n_examined, bf_keys, bf_hits, bf_misses,
        bf_examined;
    n_keys = n_hits = n_misses = n_examined = bf_keys = bf_hits = bf_misses =
        bf_examined = 0;
    ht_stats(hash, &n_keys, &n_hits, &n_misses, &n_examined);
    bf_stats(boyf, &bf_keys, &bf_hits, &bf_misses, &bf_examined);
    print_stats(n_keys, n_hits, n_misses, n_examined, bf_keys, bf_hits,
                bf_misses, bf_examined, bf_count(boyf), bf_size(boyf));
  } 
  // if the user wants the counseling messages
  else {
    // if the user used mixed speak, print the respective message
    if (ll_length(used_bad_words) != 0 &&
        ll_length(used_old_words) != 0) {
      printf("%s", mixspeak_message);
      ll_print(used_bad_words);
      ll_print(used_old_words);
    }
    // if the user used only badspeak, print the respective message 
    else if (ll_length(used_bad_words) != 0 &&
               ll_length(used_old_words) == 0) {
      printf("%s", badspeak_message);
      ll_print(used_bad_words);
    } 
    // if the user used only oldspeak, print the respective message
    else if (ll_length(used_bad_words) == 0 &&
               ll_length(used_old_words) != 0) {
      printf("%s", goodspeak_message);
      ll_print(used_old_words);
    }
  }
  // delete the linked lists, hash table, bloom filter and parser
  ll_delete(&used_bad_words);
  ll_delete(&used_old_words);
  bf_delete(&boyf);
  ht_delete(&hash);
  parser_delete(&parse);
  return 0;
}

// takes the needed statistics from the hash table and bloom filter and uses it
// to print out the required statistics
void print_stats(uint32_t ht_k, uint32_t ht_h, uint32_t ht_m, uint32_t ht_p,
                 uint32_t bf_k, uint32_t bf_h, uint32_t bf_m, uint32_t bf_be,
                 uint32_t count, uint32_t size) {
  printf("ht keys: %u\n", ht_k);
  printf("ht hits: %u\n", ht_h);
  printf("ht misses: %u\n", ht_m);
  printf("ht probes: %u\n", ht_p);
  printf("bf keys: %u\n", bf_k);
  printf("bf hits: %u\n", bf_h);
  printf("bf misses: %u\n", bf_m);
  printf("bf bits examined: %u\n", bf_be);
  if (bf_m != 0)
    printf("Bits examined per miss: %f\n",
           ((double)bf_be - (N_HASHES * (double)bf_h)) / (double)bf_m);
  else // avoids dividing by 0
    printf("Bits examined per miss: %f\n", (double)0);
  if (bf_h != 0)
    printf("False positives: %f\n", (double)ht_m / (double)bf_h);
  else // avoids dividing by 0
    printf("False positives: %f\n", (double)0);
  if ((ht_h + ht_m) != 0)
    printf("Average seek length: %f\n",
           (double)ht_p / ((double)ht_h + (double)ht_m));
  else // avoids dividing by 0
    printf("Average seek length: %f\n", (double)0);
  printf("Bloom filter load: %f\n", (double)count / (double)size);
}

// prints the help and usage information for banhammer
void print_help(void) {
  fprintf(stderr, "Usage: ./banhammer [options]\n");
  fprintf(stderr, "  ./banhammer will read in words from stdin, identify any badspeak "
         "or old speak and output an\n");
  fprintf(stderr, "  appropriate punishment message. The badspeak and oldspeak (with "
         "the newspeak translation)\n");
  fprintf(stderr, "  that caused the punishment will be printed after the message. If "
         "statistics are enabled\n");
  fprintf(stderr, "  punishment messages are suppressed and only statistics will be "
         "printed.\n");
  fprintf(stderr, "    -t <ht_size>: Hash table size set to <ht_size>. (default: 10000)\n");
  fprintf(stderr, "    -f <bf_size>: Bloom filter size set to <bf_size>. (default 2^19)\n");
  fprintf(stderr, "    -s          : Enables the printing of statistics.\n");
  fprintf(stderr, "    -m          : Enables move-to-front rule.\n");
  fprintf(stderr, "    -h          : Display program synopsis and usage.\n");
}
