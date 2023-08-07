#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef __APPLE__
#include <stdlib.h>
#define randomInt(n) (arc4random_uniform((n)))
#else
#define randomInt(n) (rand() % (n))
#endif

#define MAX_LINE_SIZE 1024
#define MAX_WORD_SIZE 20
char *initBoard(char *puzzleState, char *randomWord) {
  printf("Welcome to Hangman!\n");
  printf("Guess a letter\n");
  int i;
  for (i = 0; i < strlen(randomWord); i++) {
    puzzleState[i] = '_';
  }
  puzzleState[i] = '\0';

  return puzzleState;
}

char **readFromFile(int *word_count) {
  FILE *csvFile = fopen("res/words.csv", "r");
  char **words = NULL;

  if (!csvFile) {
    perror("Error opening file");
    return NULL;
  }

  char lineBuffer[MAX_LINE_SIZE]; // Buffer that holds every Line of characters
  char *token; // Buffer that holds each String, split by commas

  while (fgets(lineBuffer, sizeof(lineBuffer), csvFile)) {
    token = strtok(lineBuffer, ",");
    while (token) {
      *word_count += 1;

      words =
          realloc(words, (*word_count) *
                             sizeof(char *)); // Allocates more memory to hold
                                              // pointers in the words array
      words[*word_count - 1] = malloc(
          strlen(token) + 1); // Allocates memory for the word itself. The +1 is
                              // for the end of string character. ("\0")
      strcpy(words[*word_count - 1], token);

      token = strtok(NULL, ",");
    }
  }
  fclose(csvFile);
  return words;
}
void freeWordAllocation(char **words, int count) {
  for (int i = 0; i < count; i++) {
    free(words[i]);
  }
  free(words);
}
char *getRandomWord(char **words, int wordCount) {

  int randomIndex = randomInt(wordCount);
  // int randomIndex = random() % wordCount;
  // printf("Random index: %d\n", randomIndex);
  return words[randomIndex];
}
bool checkAnswer(char guess, char *word, char *puzzleState, int *correctCount) {
  bool found = false;
  int length = strlen(word);

  for (int i = 0; i < length; i++) {
    if (word[i] == guess) {
      if (puzzleState[i] == '_') {
        (*correctCount)++;
      }
      puzzleState[i] = guess;

      found = true;
    }
  }

  return found;
}
void finishGame(bool result, char *word) {
  if (result) {
    printf("\n");
    printf("- - - - - - - - -\n");
    printf("CONGRATULATIONS! You have won!");
  } else {
    printf("\n");
    printf("- - - - - - - -\n");
    printf("The word was: %s\n", word);
    printf("Better luck next time :)");
  }
}
int main() {
  int wordCount = 0;
  char **wordsFromFile = readFromFile(&wordCount);
  char guessedChar;
  int failCount = 0;
  int correctCount = 0;
  bool gameCompleted = false;
  srand(time(NULL));
  char *randomWord = getRandomWord(wordsFromFile, wordCount);
  char *puzzleState = malloc((strlen(randomWord) + 1) * sizeof(char));
  if (!puzzleState) {
    printf("Memory Allocation failed");
  }

  puzzleState = initBoard(puzzleState, randomWord);

  // printf("Word length: %lu\n", strlen(randomWord));

  while (failCount < 7 && !gameCompleted) {
    printf("Fail Count: %d of 7\n", failCount);
    printf("Correct: %d of %lu\n", correctCount, strlen(randomWord));
    printf("=> %s\n", puzzleState);
    printf("Guess: ");
    scanf(" %c", &guessedChar);

    bool checkanswer =
        checkAnswer(guessedChar, randomWord, puzzleState, &correctCount);
    if (checkanswer != true) {
      failCount++;
      if (failCount == 7) {
        finishGame(false, randomWord);
      }
    } else if (correctCount == strlen(randomWord)) {
      gameCompleted = true;
      printf("=> %s\n", puzzleState);
      finishGame(true, randomWord);
    }
  }
  free(puzzleState);
  freeWordAllocation(wordsFromFile, wordCount);

  return 0;
}
