#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_TRIES 6
#define MAX_WORD_LENGTH 26

// ANSI color codes
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_GRAY    "\x1b[90m"

int check_guess(char guess[], char target[]);
void print_word(char word[], int length);
void filter_words(char **words, int word_count, char filter[], int word_length);

int main()
{
    while (true) {
        printf("1. Play Wordle\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            char target[MAX_WORD_LENGTH + 1], guess[MAX_WORD_LENGTH + 1];
            int correct_letter;
            int try = 1;
            int play_again = 1;

            FILE *fp;
            fp = fopen("bdd_wordle.txt", "r");
            if (fp == NULL) {
                printf("Error opening file.\n");
                return 1;
            }

            // Count the number of words in the file
            int word_count = 0;
            char word[MAX_WORD_LENGTH + 1];
            while (fscanf(fp, "%s", word) != EOF) {
                word_count++;
            }

            // Allocate memory for the words
            char **words = malloc(word_count * sizeof(char *));
            if (words == NULL) {
                printf("Memory allocation error.\n");
                return 1;
            }

            // Rewind the file pointer to read the words
            rewind(fp);

            // Read words from the file
            for (int i = 0; i < word_count; i++) {
                words[i] = malloc(MAX_WORD_LENGTH + 1);
                if (words[i] == NULL) {
                    printf("Memory allocation error.\n");
                    return 1;
                }
                fscanf(fp, "%s", words[i]);
            }

            fclose(fp);

            printf("Welcome to Wordle!\n");
            printf("Instructions:\n");
            printf("The color of a letter will change to show you how close your guess was.\n");
            printf("If the letter is green, the letter is in the word and it is in the correct spot.\n");
            printf("If the letter is yellow, the letter is in the word, but it is not in the correct spot.\n");
            printf("If the letter is grey, the letter is not in the word.\n");
            printf("You will have 6 guesses. Good luck!\n\n");

            while (play_again) {
                // Select target word randomly
                srand(time(NULL));
                int index = rand() % word_count;
                strcpy(target, words[index]);

                int word_length = strlen(target);
                try = 1;

                printf("The word you are guessing has %d letters.\n\n", word_length);

                // Game loop
                while (try <= MAX_TRIES) {
                    printf("Try %d: Enter a word (or 'q' to quit): ", try);
                    scanf("%s", guess);

                    // Check if user wants to quit
                    if (strcmp(guess, "q") == 0) {
                        play_again = 0;
                        break;
                    }

                    if (strlen(guess) != word_length) {
                        printf("Word must be %d letters long. Try again.\n", word_length);
                        continue;
                    }

                    correct_letter = check_guess(guess, target);

                    if (correct_letter == word_length) {
                        printf("Congratulations! You found the word '%s' in %d tries.\n", target, try);
                        break;
                    }

                    print_word(guess, word_length);
                    putchar('\n');

                    int want_hint;
                    printf("Do you want a hint? (1 = Yes, 0 = No): ");
                    scanf("%d", &want_hint);
                    if (want_hint) {
                        char filter[MAX_WORD_LENGTH + 1];
                        printf("Enter the filter (use * for unknown letters): ");
                        scanf("%s", filter);
                        filter_words(words, word_count, filter, word_length);
                    }

                    try++;
                }

                if (strcmp(guess, "q") == 0) {
                    break;
                }

                // Display the target word at the end of the game
                printf("The word was: '%s'\n", target);

                // Ask if the player wants to play again
                printf("Do you want to play again? (1 = Yes, 0 = No): ");
                scanf("%d", &play_again);
            }

            printf("Thanks for playing!\n");

            // Free memory allocated for words
            for (int i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
        } else if (choice == 2) {
            printf("Exiting Wordle...\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

int check_guess(char guess[], char target[]) {
    int i, correct_letter = 0;
    for (i = 0; i < strlen(target); i++) {
        if (guess[i] == target[i]) {
            correct_letter += 1;
            printf(ANSI_COLOR_GREEN "%c" ANSI_COLOR_RESET, guess[i]);
        } else if (strchr(target, guess[i]) != NULL) {
            printf(ANSI_COLOR_YELLOW "%c" ANSI_COLOR_RESET, guess[i]);
        } else {
            printf(ANSI_COLOR_GRAY "%c" ANSI_COLOR_RESET, guess[i]);
        }
    }
    putchar('\n');
    return correct_letter;
}

void filter_words(char **words, int word_count, char filter[], int word_length) {
    printf("Words matching '%s':\n", filter);
    for (int i = 0; i < word_count; i++) {
        int match = 1;
        for (int j = 0; j < word_length; j++) {
            if (filter[j] != '*' && words[i][j] != filter[j]) {
                match = 0;
                break;
            }
        }
        if (match) {
            printf("%s\n", words[i]);
        }
    }
}

void print_word(char word[], int length)
{
    for (int i = 0; i < length; i++) {
        putchar(word[i]);
        putchar(' ');
    }
    putchar('\n');
}

