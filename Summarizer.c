/* This program will summarize the text of an article */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sysexits.h>
#include "Summarizer.h"

static void get_sentences(char *[], char[], int * count);

int main(int argc, char *argv[]) {

  FILE * input;
  Word_Map *map[3000]; /* Array of pointers to structs (max 3000 words) */
  char * sentences[100]; /* Hold all (100) sentences of article in order */
  char text[5000]; /* Array that holds read in paragraphs (max 5000 chars) */
  int sentence_count = 0; /* number of sentences */

  if (argc == 1) {
    /* inplement pasting into terminal case later */

  } else if (argc == 2) {
    input = fopen(argv[1], "r");
    if (input == NULL) {
      fprintf(stderr, "%s cannot be opened.\n", argv[1]);
      exit(EX_OSERR);
    }
    while (fgets(text, 15000, input) != NULL) { /* Read in each paragraph */
      /* process each paragraph */
      get_sentences(sentences, text, &sentence_count);
    }
    printf("number of sentences: %d\n", sentence_count);
  } else { /* Incorrect input */
    fprintf(stderr, "Usage: a.out");
    fprintf(stderr, "Usage: a.out <filename>\n");
    exit(EX_USAGE);
  }
  return 0;
}

static void get_sentences(char * arr[], char * text, int * count) {
  char sentence[5000]; /* Holds the sentence */
  int i, j = 0, length = strlen(text), quotes_seen = 0;
  int end_sentence = 0;

  for (i = 0; i < length; i++) {
    while (isspace(text[i]) && j == 0) { /* Skip over preceding whitespace */
        i++;
    }

    /* If current character is a quotation mark */
    if (text[i] == '\"' || text[i] == -100 || text[i] == -99) {
      quotes_seen++;
    }
    sentence[j++] = text[i]; /* Copy text over normally otherwise */

    /* End of sentence */
    if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
      if (quotes_seen % 2 != 0) { /* If not all quotes copied over */
        sentence[j++] = '"'; /* Add end quote */
        /* Case: End of quote != end of sentence
        if (text[i] != '.' || text[i] != '!' || text[i] != '?') {
          end_sentence = 0;
        }*/
      }
      sentence[j] = '\0'; /* Null character to make string */
      j = 0;
      (*count)++; /* Increment # of sentences */

      printf("%s\n\n", sentence);
      memset(sentence, 0, sizeof(sentence));
    }
  }
}
