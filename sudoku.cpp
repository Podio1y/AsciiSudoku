#include <iostream>

int main();
void printBoard(int*);
bool checkIfHidden(int*, int, int);
int userInput(int*);
void unhide(int*, int);
bool isGameWon(int*);
bool isCorrectGuess(int*, int);

int main(){

    int * board = new int [81]; // Board array
    int choice = 0;
    int lives = 3;

    // For loop for testing the printBoard funtion. Tests to see if printing hidden/unhidden values properly
    for (int i = 0 ; i < 81 ; i++){
        board[i] = (i % 9) + 1;
        if (i % 9 != 0){
            board[i] = board[i] ^ 0b00010000;
        }
    }

    while(lives > 0){
        system("cls");
        printBoard(board);

        choice = userInput(board);
        
        while (!isCorrectGuess(board, choice)){
            std::cout << "That can't go there!" << std::endl;
            lives--;
            choice = userInput(board);
        }

        unhide(board, choice);
    }
    //std::cout << checkIfHidden(board, 2, 8);

    delete board;
    board = nullptr;
}

bool isCorrectGuess(int * board, int choice){
    return true;
}

bool isGameWon(int * board){

    for (int i = 0 ; i < 81 ; i++){
        if ((board[i] & 0b00010000) != 0){
            return false;
        }
    }

    return true;
}

void unhide(int * board, int index){
    board[index] = board[index] ^ 0b00010000;
}

int userInput(int * board){
    int x = 0;
    int y = 0;
    bool valid = false;

    do
    {
        std::cout << "Please input your x coordinate: " << std::endl;
        std::cin >> x;

        std::cout << "Please input your y coordinate: " << std::endl;
        std::cin >> y;

        valid = (x < 10 && x > 0) && (y < 10 && y > 0) && (!checkIfHidden(board, x, y));

        while (valid){
            std::cout << "Sorry that tile is invalid, or has already been revealed!" << std::endl;
            std::cout << "Please try again..." << std::endl;

            std::cout << "Please input your x coordinate: " << std::endl;
            std::cin >> x;

            std::cout << "Please input your y coordinate: " << std::endl;
            std::cin >> y;
            system("cls");
        }
    }
    while (valid);
    
    return (x + (y-1)*9 - 1);
}

bool checkIfHidden(int * board, int x, int y){
    return ( (board[x + (y-1)*9 - 1 ] & 0b00010000) == 0b00010000); // Returns true if hidden, false if not
}

void printBoard(int * board){

    int hiddenMask = 0b00010000; // Bit to check if the board value at a specific loction is hidden or not
    int count = 0;
    int columnCount = 1;

    std::cout << "    1  2  3   4  5  6   7  8  9" << std::endl;
    for (int j = 0 ; j < 13 ; j++){

        if (j % 4 == 0){
            std::cout << "  +---------+---------+---------+";
        }
        else{
            std::cout << columnCount << " ";
            columnCount++;

            for (int i = 0 ; i < 13 ; i++){

                if (i % 4 == 0){
                    std::cout << "|";
                }
                else{
                    if ((board[count] & hiddenMask) == hiddenMask){
                        std::cout << " . ";
                    }
                    else{
                        std::cout << " " << board[count] << " ";
                    }
                    count++;
                }
            }
        }
        std::cout << std::endl;
    }
}