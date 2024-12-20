#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // To check type

//  Initialise consts
const int ROWS = 6;
const int COLS = 7;

//  Declare functions
int mainMenu();  // Displays main menu

void newGame();  // Runs the game

void saveGame(char board[ROWS][COLS], char playerX[], char playerO[], int gameID, int emptySpaces);  // Saves the game to file

void secondMenu();  // Displays second menu

void displayBoard(char board[ROWS][COLS]); // Displays the game board

int makeMove(char board[ROWS][COLS], int col, char player); // Places a token

int isWinningMove(char board[ROWS][COLS], char player, int winCoords[]); // Checks for a win

int isDraw(char board[ROWS][COLS]); // Checks for a draw

void highlightWinningMove(char board[ROWS][COLS], const int winCoords[]); // Highlights the winning move with Y

int generateGameID(); // Generates a unique game ID

void listAllSavedGames(); // Lists all saved games

void listGamesForPlayer(char playerName[]); // Lists games for a specific player

void showBoardForGameID(int gameID); // Shows the board for a specific game ID

void loadGameByID(int gameID); // Loads a game by ID


int menuOption;
char playerX[50];
char playerO[50];
int moveCounter = 0;  //num of free fields on board = emptySpaces(initially 42) - moveCounter


int main()
{
    // MAIN GAME LOOP
    while(1) {
        menuOption = mainMenu();

        if(menuOption == 1)
            newGame();
        else if(menuOption == 2)
            secondMenu();
        else if(menuOption == 3) {
            printf("\n"
                   "Thanks for playing. Bye!\n");
            break;
        }
        else
            printf("No such option. Try again\n");
    }

    return 0;
}

int mainMenu() {
    printf("\n"
           "1. New game\n"
           "2. Load\n"
           "3. Exit\n"
           "\nSelect one of the options by inputting a number: ");

    if (scanf("%d", &menuOption) != 1) { // Check if scanf fails to read an integer
        while (getchar() != '\n'); // Clear the input buffer
        return -1; // Return an invalid option indicator
    }

    return menuOption;
}

void secondMenu() {
    int loadOption;
    while(1) {
        printf("\n"
               "1. List saved games\n"
               "2. List saved games for a player (specify Player Name)\n"
               "3. Display the board for a saved game (specify game ID)\n"
               "4. Load a game (specify ID)\n"
               "5. Return to main menu\n"
               "\nSelect one of the options by inputting a number: ");
        if (scanf("%d", &loadOption) != 1) { // Check if scanf fails to read an integer
            while (getchar() != '\n'); // Clear the input buffer
            printf("No such option. Try again\n");
            continue; // Restart the loop to prompt the user again
        }
        if (loadOption == 1)
            listAllSavedGames();
        else if (loadOption == 2) {
            printf("Enter player name: ");
            char playerName[50];
            scanf("%s", playerName);
            listGamesForPlayer(playerName);
        } else if (loadOption == 3) {
            printf("Enter game ID: ");
            int gameID;
            if (scanf("%d", &gameID) != 1) {
                while (getchar() != '\n');
                printf("Invalid input. Try again.\n");
                continue;
            }
            showBoardForGameID(gameID);
        } else if (loadOption == 4) {
            printf("Enter game ID to load: ");
            int gameID;

            if (scanf("%d", &gameID) != 1) {
                // Clear invalid input from the buffer
                while (getchar() != '\n');
                printf("Invalid input. Try again.\n");
                continue;
            }

            // Directly call the load function
            loadGameByID(gameID);


        } else if (loadOption == 5) {
            break;
        } else {
            printf("Invalid option. Try again.\n");
        }

    }
}

void newGame() {
    char board[ROWS][COLS];
    int emptySpaces = 42;
    // Initialize board with dots
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '.'; // Dot for empty space
        }
    }

    printf("\nEnter player X name: ");
    while (1) {
        scanf("%s", playerX);
        if (strchr(playerX, ',') || strlen(playerX) > 50) {  // check for comma in the name
            printf("Names cannot contain a comma (,) or be longer than 50 characters. Please enter a valid name: ");
        } else {
            break; // Valid name entered
        }
    }

    printf("\nEnter player O name: ");
    while (1) {
        scanf("%s", playerO);
        if (strchr(playerO, ',') || strlen(playerO) > 50) {
            printf("Names cannot contain a comma (,) or be longer than 50 characters. Please enter a valid name: ");
        } else {
            break; // Valid name entered
        }
    }
    printf("\n%s (X) vs %s (O)\n", playerX, playerO);

    char currentPlayer = 'X';
    int col, gameID = generateGameID();
    int winCoords[8]; // To store coordinates of winning move
    int gameRunning = 1; // flag

    while (gameRunning) {
        displayBoard(board);
        printf("Player %c (%s), enter a column (1-7, 0 to save): ", currentPlayer, currentPlayer == 'X' ? playerX : playerO);

        if (scanf("%d", &col) != 1) { // Validate input
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a number between 0 and 7.\n");
            continue;
        }

        if (col == 0) {
            saveGame(board, playerX, playerO, gameID, emptySpaces);
            printf("\nGame saved with ID: %d\n", gameID);
            continue; // Continue the game after saving
        }

        if (col < 1 || col > 7) {
            printf("Invalid column. Please enter a number between 1 and 7.\n");
            continue;
        }

        if (!makeMove(board, col - 1, currentPlayer)) { // Attempt to make the move
            printf("Column %d is full. Try another column.\n", col);
            continue;
        }

        if (isWinningMove(board, currentPlayer, winCoords)) { // Check for win
            displayBoard(board);
            highlightWinningMove(board, winCoords);
            printf("Player %c (%s) wins!\n", currentPlayer, currentPlayer == 'X' ? playerX : playerO);
            gameRunning = 0; // End the game
        } else if (isDraw(board)) { // Check for draw
            displayBoard(board);
            printf("It's a draw!\n");
            gameRunning = 0; // End the game
        } else {
            // Switch players
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    // After game ends bring up a menu
    int choice;
    while (1) {
        printf("\nWhat would you like to do next?\n");
        printf("1. Start a new game\n");
        printf("2. Return to main menu\n");
        printf("Select an option: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter 1 or 2.\n");
            continue;
        }

        if (choice == 1) {
            newGame(); // Start a new using recursion
            break;
        } else if (choice == 2) {
            break; // Return to main menu
        } else {
            printf("Invalid option. Please enter 1 or 2.\n");
        }
    }
}

void displayBoard(char board[ROWS][COLS]) {
    printf("\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("| %c ", board[i][j]);
        }
        printf("|\n");
    }
    printf("  1   2   3   4   5   6   7\n"); // Column numbers for reference
}

void saveGame(char board[ROWS][COLS], char playerX[], char playerO[], int gameID, int emptySpaces) {
    FILE *file = fopen("result.txt", "a");
    if (!file) {
        printf("Error saving game. Could not open file.\n");
        return;
    }

    fprintf(file, "\n%d, %s, %s, %d\n", gameID, playerX, playerO, emptySpaces - moveCounter); // Last argument is the number of empty spaces
    fprintf(file, "Board:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(file, "%c", board[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "----\n");
    fclose(file);

    printf("\nGame successfully saved.");
}

int makeMove(char board[ROWS][COLS], int col, char player) {
    for (int i = ROWS - 1; i >= 0; i--) {  // Starts from bottom of the column and goes to the top looking for an empty space
        if (board[i][col] == '.') {
            board[i][col] = player;
            moveCounter += 1;
            return 1; // Move successful
        }
    }
    return 0; // Column full
}

int isWinningMove(char board[ROWS][COLS], char player, int winCoords[]) {
    // Horizontal check
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == player && board[i][j + 1] == player &&
                board[i][j + 2] == player && board[i][j + 3] == player) {
                winCoords[0] = i; winCoords[1] = j;
                winCoords[2] = i; winCoords[3] = j + 1;
                winCoords[4] = i; winCoords[5] = j + 2;
                winCoords[6] = i; winCoords[7] = j + 3;
                return 1;
            }
        }
    }
    // Vertical check
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == player && board[i + 1][j] == player &&
                board[i + 2][j] == player && board[i + 3][j] == player) {
                winCoords[0] = i; winCoords[1] = j;
                winCoords[2] = i + 1; winCoords[3] = j;
                winCoords[4] = i + 2; winCoords[5] = j;
                winCoords[6] = i + 3; winCoords[7] = j;
                return 1;
            }
        }
    }
    // Diagonal checks
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == player && board[i + 1][j + 1] == player &&
                board[i + 2][j + 2] == player && board[i + 3][j + 3] == player) {
                winCoords[0] = i; winCoords[1] = j;
                winCoords[2] = i + 1; winCoords[3] = j + 1;
                winCoords[4] = i + 2; winCoords[5] = j + 2;
                winCoords[6] = i + 3; winCoords[7] = j + 3;
                return 1;
            }
        }
    }
    for (int i = 3; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == player && board[i - 1][j + 1] == player &&
                board[i - 2][j + 2] == player && board[i - 3][j + 3] == player) {
                winCoords[0] = i; winCoords[1] = j;
                winCoords[2] = i - 1; winCoords[3] = j + 1;
                winCoords[4] = i - 2; winCoords[5] = j + 2;
                winCoords[6] = i - 3; winCoords[7] = j + 3;
                return 1;
            }
        }
    }
    return 0; // No win
}

int isDraw(char board[ROWS][COLS]) {
    for (int i = 0; i < COLS; i++) {
        if (board[0][i] == '.') return 0; // If there's an empty cell in the top row
    }
    return 1; // Board full
}

void highlightWinningMove(char board[ROWS][COLS], const int winCoords[]) {
    for (int i = 0; i < 8; i += 2) {
        board[winCoords[i]][winCoords[i + 1]] = 'Y'; // Highlight winning pieces
    }
    displayBoard(board); // Show the board with highlighted winning moves
}

int generateGameID() {
    int lastID = -1; // Initialize with -1 to handle the case of an empty file
    FILE *file = fopen("result.txt", "r");

    if (!file) {
        printf("No saved games found. Starting with Game ID 0.\n");
        return 0; // If no file, start with ID 0
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Extract the first number from lines that start with a digit
        if (isdigit(line[0])) {
            int id = atoi(line); // Convert the leading number to an integer
            if (id > lastID) {
                lastID = id; // Update lastID to the largest seen ID
            }
        }
    }

    fclose(file);

    return lastID + 1; // Return the next available ID
}


void listAllSavedGames() {
    FILE *file = fopen("result.txt", "r");
    if (!file) {
        printf("No saved games found.\n");
        return;
    }

    char line[256];
    int lastIDs[100];      // Array to store unique game IDs
    long lastPositions[100]; // Array to store the file positions of the last occurrences of each ID
    int uniqueCount = 0;    // Number of unique IDs

    // First pass: Locate the last occurrence of each game ID
    while (fgets(line, sizeof(line), file)) {
        if (isdigit(line[0])) { // Check if the line starts with a digit (game metadata line)
            int id;
            if (sscanf(line, "%d", &id) == 1) {
                int found = 0;
                for (int i = 0; i < uniqueCount; i++) {
                    if (lastIDs[i] == id) {
                        lastPositions[i] = ftell(file) - strlen(line); // Update to the new position
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    lastIDs[uniqueCount] = id;                        // Store new unique ID
                    lastPositions[uniqueCount] = ftell(file) - strlen(line); // Store its position
                    uniqueCount++;
                }
            }
        }
    }

    // Second pass: Print only the last occurrences
    for (int i = 0; i < uniqueCount; i++) {
        fseek(file, lastPositions[i], SEEK_SET); // Go to the position of the last occurrence
        if (fgets(line, sizeof(line), file)) {
            printf("%s", line); // Print the metadata line
        }
    }

    fclose(file);
}


void listGamesForPlayer(char playerName[]) {
    FILE *file = fopen("result.txt", "r");
    if (!file) {
        printf("No saved games found.\n");
        return;
    }

    char line[256];
    int lastIDs[100];
    long lastPositions[100];
    int uniqueCount = 0;

    // First pass: Locate the last occurrence of each game ID for the specified player
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, playerName)) { // Check if the playerName appears in the line
            int id;
            if (sscanf(line, "%d", &id) == 1) {
                int found = 0;
                for (int i = 0; i < uniqueCount; i++) {
                    if (lastIDs[i] == id) {
                        lastPositions[i] = ftell(file) - strlen(line); // Update to the new position
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    lastIDs[uniqueCount] = id;                        // Store new unique ID
                    lastPositions[uniqueCount] = ftell(file) - strlen(line); // Store its position
                    uniqueCount++;
                }
            }
        }
    }

    // Second pass: Print only the last occurrences
    for (int i = 0; i < uniqueCount; i++) {
        fseek(file, lastPositions[i], SEEK_SET); // Go to the position of the last occurrence
        if (fgets(line, sizeof(line), file)) {
            printf("%s", line); // Print the metadata line
        }
    }

    fclose(file);
}


void showBoardForGameID(int gameID) {
    FILE *file = fopen("result.txt", "r");
    if (!file) {
        printf("No saved games found.\n");
        return;
    }

    char line[256];
    long lastPosition = -1; // Position of the last instance of the specified game ID
    int currentID;

    // First pass: Find the last occurrence of the game ID
    while (fgets(line, sizeof(line), file)) {
        if (isdigit(line[0])) { // Check if the line starts with a digit (potential game ID)
            currentID = atoi(line); // Convert the leading number to an integer
            if (currentID == gameID) {
                lastPosition = ftell(file) - strlen(line); // Store the position of this instance
            }
        }
    }

    if (lastPosition == -1) {
        printf("Game ID %d not found.\n", gameID);
        fclose(file);
        return;
    }

    // Second pass: Display the board for the last instance
    fseek(file, lastPosition, SEEK_SET); // Go to the position of the last instance
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (isdigit(line[0])) { // Check if the line starts with a digit (new game ID)
            if (found) break; // Stop when a new game ID is encountered
        } else {
            if (!found) found = 1; // Start processing the board lines
            if (line[0] == '-') break; // Stop when reaching the delimiter
            printf("%s", line); // Print board lines
        }
    }

    fclose(file);
}

// This function is extended to prevent error in the case that a game would be saved multiple times
void loadGameByID(int gameID) {
    FILE *file = fopen("result.txt", "r");
    if (!file) {
        printf("No saved games found.\n");
        return;
    }

    char board[ROWS][COLS];
    char playerX[50], playerO[50];
    char currentPlayer;
    char line[256];
    int found = 0;
    int lastEmptySpaces = -1;
    long gamePosition = 0;  // Store the position of the last found game

    // First pass: Locate the game ID and track the game with the lowest emptySpaces
    while (fgets(line, sizeof(line), file)) {
        int id, emptySpaces;
        if (sscanf(line, "%d, %50[^,], %50[^,], %d", &id, playerX, playerO, &emptySpaces) == 4) {
            if (id == gameID) {
                if (lastEmptySpaces == -1 || emptySpaces < lastEmptySpaces) {
                    // Track the position of this game
                    gamePosition = ftell(file);  // Store current position
                    lastEmptySpaces = emptySpaces;
                    printf("Found game ID %d with %d empty spaces at position %ld\n", gameID, emptySpaces, gamePosition); // Debugging output
                }
            }
        }
    }

    // If no matching game ID is found, exit
    if (gamePosition == 0) {
        printf("Game with ID %d not found.\n", gameID);
        fclose(file);
        return;
    }

    // Second pass: Load the board for the game with the lowest emptySpaces
    fseek(file, gamePosition, SEEK_SET);  // Jump to the position of the last matching game
    printf("Seeking to position %ld\n", gamePosition);  // Debugging output

    int boardLoaded = 0;
    // Locate and load the board data associated with this game
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Board:")) {
            // Now read the board
            for (int i = 0; i < ROWS; i++) {
                if (fgets(line, sizeof(line), file)) {
                    // If the line is the delimiter (i.e., "----"), break out of the loop
                    if (strstr(line, "----")) {
                        break;  // Stop reading the board data
                    }

                    // Only process valid characters (X, O, or .)
                    for (int j = 0; j < COLS; j++) {
                        if (line[j] == 'X' || line[j] == 'O' || line[j] == '.') {
                            board[i][j] = line[j];
                        } else {
                            board[i][j] = '.';  // Treat invalid characters as empty space
                        }
                    }
                } else {
                    printf("Error: Board data incomplete.\n");
                    fclose(file);
                    return;
                }
            }
            boardLoaded = 1;
            break; // Break after loading the board
        }
    }

    fclose(file);

    if (!boardLoaded) {
        printf("Error: Board data not found.\n");
        return;
    }

    // Calculate the current player based on the last saved emptySpaces value
    currentPlayer = (lastEmptySpaces & 1) == 0 ? 'X' : 'O';

    printf("Game loaded successfully: %s (X) vs %s (O)\n", playerX, playerO);

    // Start the game loop, similar to newGame
    int gameRunning = 1;
    int winCoords[8]; // To store coordinates of the winning move

    while (gameRunning) {
        displayBoard(board);  // Use the loaded board
        printf("Player %c (%s), enter a column (1-7, 0 to save): ", currentPlayer, currentPlayer == 'X' ? playerX : playerO);

        int col;
        if (scanf("%d", &col) != 1) { // Validate input
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a number between 0 and 7.\n");
            continue;
        }

        if (col == 0) {
            saveGame(board, playerX, playerO, gameID, lastEmptySpaces);
            printf("Game saved with ID: %d\n", gameID);
            continue; // Continue the game after saving
        }

        if (col < 1 || col > 7) {
            printf("Invalid column. Please enter a number between 1 and 7.\n");
            continue;
        }

        if (!makeMove(board, col - 1, currentPlayer)) { // Attempt to make the move
            printf("Column %d is full. Try another column.\n", col);
            continue;
        }

        if (isWinningMove(board, currentPlayer, winCoords)) { // Check for win
            displayBoard(board);
            highlightWinningMove(board, winCoords);
            printf("Player %c (%s) wins!\n", currentPlayer, currentPlayer == 'X' ? playerX : playerO);
            gameRunning = 0; // End the game
        } else if (isDraw(board)) { // Check for draw
            displayBoard(board);
            printf("It's a draw!\n");
            gameRunning = 0; // End the game
        } else {
            // Switch players
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    // After game ends, ask to start a new game or return to main menu
    int choice;
    while (1) {
        printf("\nWhat would you like to do next?\n");
        printf("1. Start a new game\n");
        printf("2. Return to main menu\n");
        printf("Select an option: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter 1 or 2.\n");
            continue;
        }

        if (choice == 1) {
            newGame(); // Start a new game recursively
            break;
        } else if (choice == 2) {
            break; // Return to main menu
        } else {
            printf("Invalid option. Please enter 1 or 2.\n");
        }
    }
}