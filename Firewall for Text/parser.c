#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Citations: Based string compare code off of the strcmp source code.

#define MAX_PARSER_LINE_LENGTH 1000

// returns the length of a string
uint64_t string_length(char *s) {
  uint64_t index;
  // for each character until the NULL ending character, add 1 to the length
  for (index = 0; *(s + index) != '\0'; index += 1) {
  }
  return index;
}

typedef struct Parser Parser;

struct Parser {
  FILE *f;
  char current_line[MAX_PARSER_LINE_LENGTH + 1];
  uint32_t line_offset;
};

Parser *parser_create(FILE *f) {
  // allocate memory for the parser
  Parser *p = (Parser *)malloc(sizeof(Parser));
  p->f = f; // set the parsing file to the given file
  p->line_offset = 0; // set the line offset to 0
  return p;
}

void parser_delete(Parser **p) {
  free(*p); // free the parser
  *p = NULL;
}

void copy_next_word(Parser *p, char *word, uint32_t *current_char);
void next_valid_word(Parser *p, uint32_t *current_char);

bool next_word(Parser *p, char *word) {
  // get the current character and make a word buffer
  uint32_t current_char = p->line_offset;
  char current_word[MAX_PARSER_LINE_LENGTH];
  if (p->current_line[current_char] == '\0') {
    p->line_offset = current_char = 0; 
    if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH, p->f) == NULL) {
      return false;
    }
  }
  copy_next_word(p, current_word, &current_char);
  if (p->line_offset == 0 && string_length(current_word) == 0) {
    next_valid_word(p, &current_char);
    copy_next_word(p, current_word, &current_char);
    while (string_length(current_word) == 0) {
      p->line_offset = current_char = 0; 
      if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH, p->f) == NULL) {
        return false;
      }
      next_valid_word(p, &current_char);
      copy_next_word(p, current_word, &current_char);
    }
  }
  next_valid_word(p, &current_char); 
  p->line_offset = current_char;
  uint64_t length = string_length(current_word) + 1;
  for (uint64_t character = 0; character < length; character += 1)
    word[character] = tolower(current_word[character]);
  return true;
}

// copys the next valid word to the word buffer
// this is used to not repeat code
void copy_next_word(Parser *p, char *word, uint32_t *current_char) {
  uint32_t index = 0;
  // while the current character is valid
  while (isalnum(p->current_line[*current_char]) ||
         p->current_line[*current_char] == 39 ||
         p->current_line[*current_char] == 45) {
    // copy the current character of the current line to the word buffer
    word[index] = p->current_line[*current_char];
    // increment the index and current character by 1
    index += 1;
    *current_char += 1;
  }
  // end the word with the NULL ending character '\0'
  word[index] = '\0';
}

// ends the current character at the start of the next valid word
// this is used to not repeat code
void next_valid_word(Parser *p, uint32_t *current_char) {
  // while the current character isn't valid and isn't the end of the line
  while (!isalnum(p->current_line[*current_char]) &&
         p->current_line[*current_char] != '\0' &&
         p->current_line[*current_char] != 39 &&
         p->current_line[*current_char] != 45) {
    // increment the current character by 1
    *current_char += 1;
  }
}
