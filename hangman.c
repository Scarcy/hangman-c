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
  char *words[7];
  char guessedChar;
  int failCount = 0;
  int *correctCount = malloc(sizeof(int));
  *correctCount = 0;
  bool gameCompleted = false;
  words[0] = "programmering";
  words[1] = "monitor";
  words[2] = "minecraft";
  words[3] = "strikking";
  words[4] = "langrenn";
  words[5] = "mikrofon";
  words[6] = "tastatur";
  time_t seed = 1234;
  srand(time(&seed));
  char *randomWord = getRandomWord(words, 7);
  char *puzzleState = malloc((strlen(randomWord) + 1) * sizeof(char));
  if (!puzzleState) {
    printf("Memory Allocation failed");
  }

  puzzleState = initBoard(puzzleState, randomWord);

  // printf("Word length: %lu\n", strlen(randomWord));

  while (failCount < 7 && !gameCompleted) {
    printf("Fail Count: %d of 7\n", failCount);
    printf("Correct: %d of %lu\n", *correctCount, strlen(randomWord));
    printf("=> %s\n", puzzleState);
    printf("Guess: ");
    scanf(" %c", &guessedChar);

    bool checkanswer =
        checkAnswer(guessedChar, randomWord, puzzleState, correctCount);
    if (checkanswer != true) {
      failCount++;
      if (failCount == 7) {
        finishGame(false, randomWord);
      }
    } else if (*correctCount == strlen(randomWord)) {
      gameCompleted = true;
      printf("=> %s\n", puzzleState);
      finishGame(true, randomWord);
    }
  }
  free(puzzleState);
  free(correctCount);
  return 0;
}
