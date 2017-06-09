/* This program will summarize the text of an article */

#include <stdio.h>
#include <stdlib.h>

int main() {

  printf("Welcome. This Summarizer will summarize an article.\n");
  while (1) {   
    int type;

    printf("Enter '1' to process plain text, "
           "'2' for a file, and anything else to exit: ");
    scanf(" %d", &type);
    if (type == 1) {
      printf("Paste the text here: \n");
    } else if (type == 2) {
      printf("Type the name of the file here: \n");
    } else {
      exit(0);
    }
  }
  return 0;
}

