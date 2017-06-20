#if !defined(SUMMARIZER_H)
#define SUMMARIZER_H

typedef struct word_map {
  char word[25];
  int count;
} Word_Map;

typedef struct score_index {
  int score;
  int index;
} Score_Index;

#endif
