#include <iostream>

int main();
void printBoard(int*);
bool checkIfHidden(int*, int, int);
int userInput(int*);
void unhide(int*, int);
bool isGameWon(int*);
bool isCorrectGuess(int*, int, int);
bool noDuplicatesOrZero(int*);

int main(){

    int * board = new int [81]{5,9,1,4,2,3,7,6,8,4,2,8,6,7,5,1,3,9,3,7,6,9,8,1,2,4,5,6,8,7,3,5,4,9,2,1,9,5,3,2,1,6,4,8,7,2,1,4,8,9,7,6,5,3,1,6,2,5,3,9,8,7,4,8,3,9,7,4,2,5,1,6,7,4,5,1,6,8,3,9,2}; // Board array
    int choice = 0;
    int lives = 3;
    int value = 0;

    // For loop for testing the printBoard funtion. Tests to see if printing hidden/unhidden values properly
    for (int i = 0 ; i < 81 ; i++){
        //board[i] = (i % 9) + 1;
        if (i % 9 != 0){
            board[i] = board[i] ^ 0b00010000;
        }
    }

    while(lives > 0){
        system("cls");
        printBoard(board);

        choice = userInput(board);
        
        std::cout << "Please enter your guess: " << std::endl;
        std::cin >> value;

        while (!isCorrectGuess(board, choice, value) && lives > 0){
            std::cout << "That can't go there!" << std::endl;
            lives--;
            choice = userInput(board);

            std::cout << "Please enter your guess: " << std::endl;
            std::cin >> value;
        }
        if (lives <= 0){
            break;
        }
        unhide(board, choice);
    }
    std::cout << checkIfHidden(board, 2, 8);

    delete board;
    board = nullptr;

    return 0;
}

bool noDuplicatesOrZero(int * array){

    for (int i = 0 ; i < 9 ; i++){
        //std::cout << "array: " << array[i] << "  index: " << i << std::endl;
        for (int j = i ; j < 9 ; j++){

            //std::cout << "array[i]: " << array[i] << "  array[j]: " << array[j] << std::endl;
            if (i != j){
                if ((array[i] == array[j]) || (array[i] == 0) || (array[j] == 0)){
                    return false;
                }
            }
        }
    }
    //std::cout << "--" << std::endl;
    return true;
}

bool isCorrectGuess(int * board, int choice, int value){
    //int choice = x + 1;
    int * array = new int [9]{0,0,0,0,0,0,0,0,0};

    // Columns checking
    for (int i = choice ; i > 0 ; i -= 9){
        if (i == choice){
            array[(board[i] & 0b00001111)-1] = value;
            //std::cout << "-" << array[(board[i] & 0b00001111)-1] << std::endl;
        }
        else{
            array[(board[i] & 0b00001111)-1] = board[i] & 0b00001111;
            //std::cout << "-" << array[(board[i] & 0b00001111)-1] << std::endl;
        }
    }

    for (int i = choice ; i < 81 ; i += 9){
        if (i == choice){
            array[(board[i] & 0b00001111)-1] = value;
            //std::cout << "/" << array[(board[i] & 0b00001111)-1] << "  index: " << (board[i] & 0b00001111)-1 << std::endl;
        }
        else{
            array[(board[i] & 0b00001111)-1] = board[i] & 0b00001111;
            //std::cout << "/" << array[(board[i] & 0b00001111)-1] << "  index: " << (board[i] & 0b00001111)-1 << std::endl;
        }
    }

    if (!noDuplicatesOrZero(array)){
        return false;
    }

    for (int i = 0 ; i < 9 ; i++){
        array[i] = 0;
    }

    // Rows checking
    for (int i = (choice - (choice % 9)) ; i < (choice - (choice % 9) + 9) ; i++){
        if (i == choice){
            array[(board[i] & 0b00001111)-1] = value;
            //std::cout << "." << array[(board[i] & 0b00001111)-1] << std::endl;
        }
        else{
            array[(board[i] & 0b00001111)-1] = board[i] & 0b00001111;
            //std::cout << "." << array[(board[i] & 0b00001111)-1] << std::endl;
        }
    }

    if (!noDuplicatesOrZero(array)){
        return false;
    }

    for (int i = 0 ; i < 9 ; i++){
        array[i] = 0;
    }

    // Boxes checking
    int row = 0;
    int col = 0;
    
    if ((choice % 9) < 3){
        col = 0;
    }
    else if ((choice % 10) < 6){
        col = 1;
    }
    else if ((choice % 10) < 9){
        col = 2;
    }

    if (choice < 9*3){
        row = 0;
    }
    else if (choice < 9*6){
        row = 1;
    }
    else if (choice < 9*9){
        row = 2;
    }

    //std::cout << "col: " << col << "  row: " << row << std::endl;
    int increment = 0;
    int topLeftNum = row * 27 + col * 3; // The top left number of whatever box their seleciton is
    for (int i = 0 ; i < 3 ; i++){
        array[(board[topLeftNum + i + increment] & 0b00001111) - 1] = (board[topLeftNum + i + increment] & 0b00001111);
        //std::cout << "index: " << (board[topLeftNum + i + increment] & 0b00001111) - 1 << "  value: " << (board[topLeftNum + i + increment] & 0b00001111) << std::endl;
        if (i == 2 && increment < 18){
            i = -1; // Needs to go back to -1, not 0 because 1 will be added when the for loop repeats directly after this, making it 0
            increment += 9;
        }
    }

    if (!noDuplicatesOrZero(array)){
        return false;
    }

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