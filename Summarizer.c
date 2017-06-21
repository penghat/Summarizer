/* This program will summarize the text of an article */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sysexits.h>
#include "Summarizer.h"

static void get_sentences(char arr[][5000], char arr2[], int * count);
static void get_words(Word_Map * map[], char text[], int * count);
static void score_sentences(Word_Map * map[], char * text,
                            int scores[], int * index, int word_count);
static void get_highest_sentences(int scores[], int indices[],
                                  int summary_length, int scores_length);


int main(int argc, char * argv[]) {

  FILE * input;
  Word_Map * map[3000]; // Array of pointers to structs (max 3000 words)
  char sentences[100][5000]; // Holds 100 sentences of article in order
  char text[5000]; // Holds read in paragraphs (max 5000 chars)
  int sentence_count = 0, word_count = 0; // # unique sentences/words
  int scores[100], index = 0; // Hold the scores of each sentence
  int summary_length; // The # of sentences to include in the summary
  int indices[100]; // Sentences to print in summary (highest)

  if (argc == 3) {
    input = fopen(argv[1], "r");
    if (input == NULL) { // File with text does not exist
      fprintf(stderr, "%s cannot be opened.\n", argv[1]);
      exit(EX_OSERR);
    }

    while (fgets(text, 15000, input) != NULL) { // Read in each paragraph
      get_sentences(sentences, text, &sentence_count); // Process paragraph
    }

     // Set # of sentences in final summary
    if (atoi(argv[2]) > sentence_count) {
      summary_length = sentence_count;
    } else { // # sentences user wants < # sentences (good/ok)
      summary_length = atoi(argv[2]);
    }

    for (int i = 0; i < sentence_count; i++) { // Process sentences
      get_words(map, sentences[i], &word_count);
    }
    for (int i = 0; i < sentence_count; i++) { // Score sentences
      score_sentences(map, sentences[i],
                            scores, &index, word_count);
    }
    get_highest_sentences(scores, indices, summary_length, sentence_count);

    /*for (int f = 0; f < word_count; f++) {
      printf("word: %s frequency: %d\n", (map[f])->word, map[f]->count);
    }*/
  } else { // Incorrect input
    fprintf(stderr, "Usage: a.out");
    fprintf(stderr, "Usage: a.out <filename> <# of lines in summary>\n");
    exit(EX_USAGE);
  }
  return 0;
}

static void get_sentences(char arr[][5000], char * text, int * count) {

  char sentence[5000]; // Holds the sentence
  int j = 0, length = strlen(text), quotes_seen = 0;

  for (int i = 0; i < length; i++) {
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

  int j = 0, length = strlen(text), word_found = 0;
  char word[25];

  for (int i = 0; i < length; i++) { // Get individual words

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

static void score_sentences(Word_Map * map[], char * text, int scores[],
                            int * index, int word_count) {

  int  score = 0, j = 0, length = strlen(text);
  char word[25];

  for (int i = 0; i < length; i++) { // Get individual words + scores

    while (isalpha(text[i]) || isdigit(text[i]) ||
           text[i] == '-' || text[i] == -103) {
      word[j++] = text[i++];
    }

    if (isspace(text[i])) { // End of word reached
      for (int k = 0; k < word_count; k++) { // Add count value to total
        if (strcasecmp((map[k])->word, word) == 0) {
            score += (map[k])->count;
        }
      }
      j = 0;
      memset(word, 0, sizeof(word)); // Clear word
    }
  }
  scores[(*index)++] = score; // Store total sentence score in array
}

static void get_highest_sentences(int scores[], int indices[],
                                  int summary_length, int scores_length) {

  Score_Index * map[100]; // Array of scores + their indices

  for (int i = 0; i < scores_length; i++) { // Store scores & indices in map
    map[i] = malloc(sizeof(Score_Index));
    (map[i])->score = scores[i];
    (map[i])->index = i;
  }
}
