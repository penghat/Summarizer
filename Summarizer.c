/* This program will summarize the text of an article */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sysexits.h>
#include "Summarizer.h"

static void get_sentences(char arr[][5000], char arr2[], int * count);
static void get_words(Word_Map * map[], char text[], int * count);

int main(int argc, char *argv[]) {

  FILE * input;
  Word_Map * map[3000]; // Array of pointers to structs (max 3000 words)
  char sentences[100][5000]; // Hold 100  sentences of article in order
  char text[5000]; // Array that holds read in paragraphs (max 5000 chars)
  int sentence_count = 0, word_count = 0; // # unique sentences/words
  int i;

  if (argc == 1) {
    // inplement pasting into terminal case later 

  } else if (argc == 2) {
    input = fopen(argv[1], "r");
    if (input == NULL) {
      fprintf(stderr, "%s cannot be opened.\n", argv[1]);
      exit(EX_OSERR);
    }
    while (fgets(text, 15000, input) != NULL) { // Read in each paragraph
      get_sentences(sentences, text, &sentence_count); // Process paragraph
    }
    for (i = 0; i < sentence_count; i++) { // Process sentence
      get_words(map, sentences[i], &word_count);
    }
    /*for (int f = 0; f < word_count; f++) {
      printf("word: %s frequency: %d\n", (map[f])->word, map[f]->count);
    }*/
  } else { // Incorrect input
    fprintf(stderr, "Usage: a.out");
    fprintf(stderr, "Usage: a.out <filename>\n");
    exit(EX_USAGE);
  }
  return 0;
}

static void get_sentences(char arr[][5000], char * text, int * count) {

  char sentence[5000]; // Holds the sentence
  int i, j = 0, length = strlen(text), quotes_seen = 0;

  for (i = 0; i < length; i++) {
    while (isspace(text[i]) && j == 0) { // Skip over preceding whitespace
        i++;
    }

    // If current character is a quotation mark
    if (text[i] == '\"' || text[i] == -100 || text[i] == -99) {
      quotes_seen++; // Keep track to ensure open AND close quotes added
    }
    sentence[j++] = text[i]; // Copy text over normally otherwise

    // End of sentence 
    if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
      if (quotes_seen % 2 != 0) { // If not all quotes added to sentence
        i++; // Move to next char

        while ((text[i] != '\"' || text[i] != -100 || text[i] != -99)
               && i < length) {  // Add all remaining characters
          sentence[j++] = text[i++];
        }
      }

      sentence[j] = '\0'; // Null character to make string
      (*count)++; // Increment # of sentences
      quotes_seen = 0; // Reset quote counters
      j = 0; // Reset counter for individual sentences
      strcpy(arr[*count - 1],sentence); // Store sentence in array 
      memset(sentence, 0, sizeof(sentence));  // Clear sentence
    }
  }
}

static void get_words(Word_Map * map[], char * text, int * count) {

  int i, j = 0, length = strlen(text), word_found = 0;
  char word[25];

  for (i = 0; i < length; i++) { // Get individual words 

    while (isalpha(text[i]) || isdigit(text[i]) ||
           text[i] == '-' || text[i] == -103) {
      word[j++] = text[i++];
    }

    if (isspace(text[i])) { // End of word reached
      for (int k = 0; k < *count; k++) { // See if word already added
        if (strcasecmp((map[k])->word, word) == 0) {
            (map[k])->count++;  // Increment frequency
            word_found = 1;
        }
      }
      if (word_found == 0) { // If word not added to map yet create new map
        map[*count] = malloc(sizeof(Word_Map));
        strcpy((map[*count])->word, word);
        (map[*count])->count = 1;
        (*count)++; // Increment # of unique words
      }

      word_found = 0; // Reset boolean that indicates if word already added
      j = 0;
      memset(word, 0, sizeof(word)); // Clear word
    }
  }
}
