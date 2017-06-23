/* Summarizer.c                                             *
 * Ananth Penghat                                           *
 * 23 June 2017                                             *
 * This program create a summary of the text of an article. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sysexits.h>
#include "Summarizer.h"

#define MAX_SENTENCES 100
#define MAX_WORDS 3000
#define MAX_CHARS 5000

static void get_sentences(char sentences[][MAX_CHARS], char text[],
                          int * count);
static void get_words(Word_Map * map[], char text[],
                      int * count, float * total);
static void score_sentences(Word_Map * map[], char * text,
                            int scores[], int * index, int word_count);
static void get_highest_sentences(int scores[], int indices[],
                                  int summary_length, int scores_length);
static void sort_score(Score_Index * map[], int length);
static void sort_index(Score_Index * map[], int length);
static void print_summary(char sentences[][MAX_CHARS], int indices[],
                          int summary_length, char * name,
                          float summary_total, float total);
static float get_number_words(char * text);

int main(int argc, char * argv[]) {

  FILE * input;
  Word_Map * map[MAX_WORDS]; // Array of pointers to structs of unique words
  char sentences[MAX_SENTENCES][MAX_CHARS]; // Holds sentences in order
  char text[MAX_CHARS]; // Holds read in paragraphs
  int scores[MAX_SENTENCES], index = 0; // Hold the scores of each sentence
  int indices[MAX_SENTENCES]; // Sentences to print in summary (highest)
  int summary_length; // The # of sentences to include in the summary
  int sentence_count = 0, word_count = 0; // The unique # of sentences/words
  float total_words = 0, summary_words = 0;

  if (argc == 3) {
    input = fopen(argv[1], "r");
    if (input == NULL) { // File with text does not exist
      fprintf(stderr, "%s cannot be opened.\n", argv[1]);
      exit(EX_OSERR);
    }

    while (fgets(text, MAX_CHARS, input) != NULL) { // Read in paragraphs
      get_sentences(sentences, text, &sentence_count); // Process paragraph
    }
    fclose(input);

     // Set # of sentences in final summary
    if (atoi(argv[2]) > sentence_count) {
      summary_length = sentence_count;
    } else { // # sentences user wants < # sentences (good/ok)
      summary_length = atoi(argv[2]);
    }

    for (int i = 0; i < sentence_count; i++) { // Process sentences
      get_words(map, sentences[i], &word_count, &total_words );
    }
    for (int i = 0; i < sentence_count; i++) { // Score sentences
      score_sentences(map, sentences[i],
                            scores, &index, word_count);
    }
    get_highest_sentences(scores, indices, summary_length, sentence_count);
    print_summary(sentences, indices, summary_length, argv[1],
                  summary_words, total_words);
  } else { // Incorrect input
    fprintf(stderr, "Usage: a.out");
    fprintf(stderr, "Usage: a.out <filename> <# of lines in summary>\n");
    exit(EX_USAGE);
  }
  return 0;
}

/* Read in the text of the article from the text file into a 2D array.   *
 * Keep track of the number of sentences for use throughout the program. */
static void get_sentences(char arr[][MAX_CHARS], char * text, int * count) {

  char sentence[MAX_CHARS]; // Holds the sentence
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

/* Parse the words of each sentence and keep count of their frequency. *
 * Keep track of the number of words for use throughout the program.   */
static void get_words(Word_Map * map[], char * text, int * count,
                      float * total) {

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

      (*total)++;
      word_found = 0; // Reset boolean that indicates if word already added
      j = 0;
      memset(word, 0, sizeof(word)); // Clear word
    }
  }
}

/* Use the frequency of each word in a sentence to 'score' each sentence. *
 * A score is the sum of the frequencies of the words in the sentence.    */
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

/* Find and store the 'x' number of sentences with the highest scores. *
 * Sentences with higher scores have more repeated/relevant words.     *
 * The user specifies the number of high scoring sentences to store.   */
static void get_highest_sentences(int scores[], int indices[],
                                  int summary_length, int scores_length) {

  Score_Index * map[MAX_SENTENCES]; // Array of scores + their indices

  for (int i = 0; i < scores_length; i++) { // Store scores & indices in map
    map[i] = malloc(sizeof(Score_Index));
    (map[i])->score = scores[i];
    (map[i])->index = i;
  }
  sort_score(map, scores_length); // Sort map by scores on sentences
  sort_index(map, summary_length); // Sort map by indices for printing
  for (int j = 0; j < summary_length; j++) { // Store indices of sentences
    indices[j] = map[j]->index;
  }
}

/* Bubblesort to sort the scored sentences by score in descending order. */
static void sort_score(Score_Index * map[], int length) {

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length - 1; j++) {
      if (map[j]->score < map[j+1]->score) {
        Score_Index * temp = map[j+1];
        map[j+1] = map[j];
        map[j] = temp;
      }
    }
  }
}

/* Bubblesort to sort the scored sentences by index in ascending order. *
 * Index refers to the sentence's position in the original article.     */
static void sort_index(Score_Index * map[], int length) {

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length - 1; j++) {
      if (map[j]->index > map[j+1]->index) {
        Score_Index * temp = map[j+1];
        map[j+1] = map[j];
        map[j] = temp;
      }
    }
  }
}

/* Write the summary to a text file. */
static void print_summary(char sentences[][MAX_CHARS], int indices[],
                          int summary_length, char * name,
                          float summary_total, float total) {
  FILE *output;
  char filename[100];
  float percent = 0;

  // Name of output file == 'input_summary.txt'
  for (int i = 0; i < strlen(name); i++) {
    if (name[i] == '.') {
      filename[i] = '\0';
      strcat(filename, "_summary.txt");
      break;
    }
    filename[i] = name[i];
  }

 output = fopen(filename, "w");
  for (int i = 0; i < summary_length; i++) {
    int index = indices[i];
    fputs(sentences[index], output);
    summary_total += get_number_words(sentences[index]);
    if (i + 1 != summary_length) {
      fputs("\n\n", output);
    }
  }
  printf("The summary has been written to the file: %s.\n", filename);
  printf("The original article was %.2f words long.\n", total);
  printf("The summary is %.2f words long.\n", summary_total);
  percent = 100 * ((total - summary_total)/total);
  printf("The summary shortened the article by %.2f percent.\n", percent);
  fclose(output);
}

static float get_number_words(char * text) {
  int j = 0, length = strlen(text);
  float total = 0;
  char word[25];

  for (int i = 0; i < length; i++) { // Get individual words

    while (isalpha(text[i]) || isdigit(text[i]) ||
           text[i] == '-' || text[i] == -103) {
      word[j++] = text[i++];
    }

    if (isspace(text[i])) { // End of word reached
      total++;
      j = 0;
      memset(word, 0, sizeof(word)); // Clear word
    }
  }
  return total;
}
