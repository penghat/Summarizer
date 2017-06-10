/* This program will summarize the text of an article */

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include "Summarizer.h"


int main(int argc, char *argv[]) {

  FILE *input;
  Word_Map *map[3000]; /* Array of pointers to structs (max 3000 words) */
  char *sentences[100]; /* Hold all sentences of article in order */
  char text[5000]; /* Array that holds read in paragraphs */

  if (argc == 1) {
    /* inplement pasting into terminal case later */

  } else if (argc == 2) {
    input = fopen(argv[1], "r");
    if (input == NULL) {
      fprintf(stderr, "%s cannot be opened.\n", argv[1]);
      exit(EX_OSERR);
    }
    while (!feof(input)) { /* Read in each paragraph */
      fgets(text, 15000, input);
      /* process each paragraph */
    }
  } else { /* Incorrect input */
    fprintf(stderr, "Usage: a.out");
    fprintf(stderr, "Usage: a.out <filename>\n");
    exit(EX_USAGE);
  }
  return 0;
}
