#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printFileContents(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int gameID;
    char playerX[50], playerO[50];
    char board[6][7]; // Assuming 6 rows and 7 columns for Connect 4
    char buffer[10];

    // Read Game ID
    if (fscanf(file, "Game ID: %d\n", &gameID) != 1) {
        printf("Error reading Game ID\n");
        fclose(file);
        return;
    }

    // Read Player X
    if (fscanf(file, "Player X: %49s\n", playerX) != 1) {
        printf("Error reading Player X\n");
        fclose(file);
        return;
    }

    // Read Player O
    if (fscanf(file, "Player O: %49s\n", playerO) != 1) {
        printf("Error reading Player O\n");
        fclose(file);
        return;
    }

    // Read "Board:" label
    if (fscanf(file, "%s\n", buffer) != 1 || strcmp(buffer, "Board:") != 0) {
        printf("Error reading Board label\n");
        fclose(file);
        return;
    }

    // Read the board
    for (int i = 0; i < 6; i++) {
        if (fscanf(file, "%6s\n", board[i]) != 1) {
            printf("Error reading board row %d\n", i + 1);
            fclose(file);
            return;
        }
    }

    fclose(file);

    // Print the results
    printf("Game ID: %d\n", gameID);
    printf("Player X: %s\n", playerX);
    printf("Player O: %s\n", playerO);
    printf("Game Board:\n");
    for (int i = 0; i < 6; i++) {
        printf("%s\n", board[i]);
    }
}

int main() {
    const char *filename = "result.txt";
    printf("Reading file: %s\n\n", filename);
    printFileContents(filename);
    return 0;
}