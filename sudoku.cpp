#include <iostream>

int main();
void printBoard(int*);

int main(){

    int * board = new int [81]; // Board array

    // For loop for testing the printBoard funtion. Tests to see if printing hidden/unhidden values properly
    for (int i = 0 ; i < 81 ; i++){
        board[i] = (i % 9) + 1;
        if (i % 9 != 0){
            board[i] = board[i] ^ 0b00010000;
        }
    }

    printBoard(board);

    delete board;
    board = nullptr;
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