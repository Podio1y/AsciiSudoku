#include <iostream>
#include <chrono>
#include <random>
#include <conio.h>

// Working on following bugs/features:
// 1. Gives an extra round before stopping after lives hit 0.

// Function Declarations
int main();
void printBoard(int*, int, int *&); // Prints the board, only showing unhidden elements
bool checkIfHidden(int*, int, int); // Checks if a certain part of the board is hidden
int userInput(int*, int, int *&); // Gets user input for which slot in the board they wish to guess a number for
void unhide(int*, int, int *&); // Un-hides the element on the board at a specified position
bool isGameWon(int*); // Checks if the game is won
bool isCorrectGuess(int*, int, int); // Checks if the guess was correct
bool noDuplicatesOrZero(int*); // Checks if there are no duplicates or zeros in the array passed in
int* generateBoard(); // Returns a randomly generated sudoku board
void removeElement(int*&, int, int); // Removes an element at a specified position in an array
int addElement(int*&, int, int); // Adds an element at a specified position to an array
bool validPlacement(int*, int, int); // Checks to see if a number can be palced a a certain position on the board
                                     // safely, without breaking sudoku rules. This is used in generating the board.
void printArray(int*, int); // Prints the first x elements of the array passed in
void removeElementByValue(int*&, int, int); // Removes the element with the value specified from the array passed in
bool isPresent(int*, int, int); // Checks if a value is present in an array or not
bool validRow(int*, int); // Checks if the row has any incorrect placements that break sudoku rules
void runSudokuGame(); // Runs the sudoku game
void hideBoard(int *&, int, int *&); // Hides a certain amount of elements on the board depending on the difficulty value passed in
void menu(char&); // Displays the menu and takes input
void goodbye(); // Goodbye message
void instructions(); // Instructions page
void letterInput(char&); // Takes any input and converts it to capital letter if necessary
void setInitialUsageValues(int * board, int *& usageCount);

int main(){

    char input = 0;
    
    while (input != 'E'){
        menu(input);
    }
    goodbye();

    return 0;
}

void instructions(){
    char pauseProgram = 0;
    system("cls");
    std::cout << "Instructions: " << std::endl;
    std::cout << "\n\t1. Your goal is to fill in every slot on the 9x9 board with numbers ranging from 1-9." << std::endl;
    std::cout << "\n\t2. There can never be the same number in any row, column, or box." << std::endl;
    std::cout << "\n\t\t(This means that each row, column, and box must have all numbers from 1-9)" << std::endl;
    std::cout << "\n\t3. You will start with numbers already revealed, harder difficulties start with less.\n" << std::endl;
    std::cout << "\n\tGOOD LUCK HAVE FUN!!!\n\n";

    letterInput(pauseProgram);
}

void goodbye(){
    char endProgram = 0;

    std::cout << "\n\nHope you enjoyed your game!!" << std::endl;
    std::cout << "\n\nPress any key to exit the program..." << std::endl;

    letterInput(endProgram);
}

void menu(char & input){
    system("cls");
    std::cout << " [P]lay - [I]nstructions - [E]xit" << std::endl;

    letterInput(input);

    while (input != 'P' && input != 'I' && input != 'E'){
        std::cout << "Sorry that was an invalid input.\n";
        std::cout << "Please try again...\n";

        letterInput(input);
        system("cls");
        std::cout << " [P]lay - [I]nstructions - [E]xit" << std::endl;
    }

    if (input == 'P'){
        runSudokuGame();
    }
    else if (input == 'I'){
        instructions();
    }
}

void letterInput(char& input){
    input = getch();
    input = (input > 90) ? (input - 32) : input;
}

void runSudokuGame(){
    //int * board = new int [81]{5,9,1,4,2,3,7,6,8,4,2,8,6,7,5,1,3,9,3,7,6,9,8,1,2,4,5,6,8,7,3,5,4,9,2,1,  9,5,3,8,1,6,4,8,7  ,2,1,4,8,9,7,6,5,3,1,6,2,5,3,9,8,7,4,8,3,9,7,4,2,5,1,6,7,4,5,1,6,8,3,9,2}; // Board array
    int * board = new int [81];
    // int * testBoard = new int[81]{5,9,0,4,2,3,7,6,8,4,2,8,6,7,5,1,3,9,3,7,6,9,8,1,2,4,5,6,8,7,3,5,4,9,2,1,9,5,3,2,1,6,4,8,7,2,1,4,8,9,7,6,5,3,1,6,2,5,3,9,8,7,4,8,3,9,7,4,2,5,1,6,7,4,5,1,6,8,3,9,2};
    int choice = 0;
    int lives = 3;
    int value = 0;
    int * usageCount = new int [9]{0,0,0,0,0,0,0,0,0};

    for (int i = 0 ; i < 81 ; i++){
        board[i] = 0;
    }

    // Generating the board
    board = generateBoard();
    hideBoard(board, 9, usageCount);
    setInitialUsageValues(board, usageCount);

    // Game Loop
    while(lives > 0){
        system("cls");
        printBoard(board, lives, usageCount);

        choice = userInput(board, lives, usageCount);
        
        std::cout << "Please enter your guess: " << std::endl;
        std::cin >> value;

        while ((!isCorrectGuess(board, choice, value) && lives > 0)){
            system("cls");
            std::cout << "That can't go there!" << std::endl;
            lives--;
            printBoard(board, lives, usageCount);
            choice = userInput(board, lives, usageCount);

            std::cout << "Please enter your guess: " << std::endl;
            std::cin >> value;
        }
        if (lives <= 0 || isGameWon(board)){
            break;
        }
        unhide(board, choice, usageCount);
    }

    delete board;
    board = nullptr;
}

void hideBoard(int *& board, int difficulty, int *& usageCount){

    // Random seed for the modern random engine
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();

    // Random engine
    std::default_random_engine eng(seed);

    int * slots = new int[9]{0,0,0,0,0,0,0,0,0};
    int count = 0;

    for (int j = 0 ; j < 9 ; j++){
        while (count < 4){
            for (int i = 0 ; i < 9 ; i++){
                if ((eng() % difficulty < 4) && slots[i] != 1){
                    count++;
                    slots[i] = 1;
                }
                if (count > 3){
                    break;
                }
            }
        }

        count = 0;
        for (int i = 0 ; i < 9 ; i++){
            if (slots[i] != 1){
                board[9*j + i] = board[9*j + i] ^ 0b00010000;
            }
        }


        delete[] slots;
        slots = nullptr;
        slots = new int[9]{0,0,0,0,0,0,0,0,0};
    }
    
    delete[] slots;
    slots = nullptr;
}

void setInitialUsageValues(int * board, int *& usageCount){

    for (int j = 0 ; j < 81 ; j++){

        if (board[j] <= 9){
            usageCount[ board[j] - 1 ]++;
        }
    }
}

bool isPresent(int * array, int size, int element){
    for (int i = 0 ; i < size ; i++){
        if (element == array[i]){
            return true;
        }
    }
    return false;
}

void removeElementByValue(int * array, int element, int size){
    for (int i = 0 ; i < size ; i++){
        if (array[i] == element){
            removeElement(array, size, i);
        }
    }
}

int * generateBoard(){
    // Random seed for the modern random engine
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();

    // Random engine
    std::default_random_engine eng(seed);

    // Stores final board
    int * board = new int[81];

    // Stores temporary numbers
    int * nums = new int[9]{1,2,3,4,5,6,7,8,9};
    int numsSize = 9;
    
    // Vars to measure the time it takes to generate the board, and to reset if it exceeds a certain limit.
    auto generateStart = std::chrono::steady_clock::now();
    auto generateFinish = std::chrono::steady_clock::now();
    double elapsedTime = std::chrono::duration_cast < std::chrono::duration<double> >(generateFinish - generateStart).count();

    while (true){
    
        generateStart = std::chrono::steady_clock::now();

        // Clears board
        for (int i = 0 ; i < 81 ; i++){
            board[i] = 0;
        }

        int count = 0;
        int randomIndex = 0;
        
        for (int j = 0 ; j < 9 ; j++){
            for (int i = 0 ; i < 9 ; i++){
                
                do {
                    randomIndex = (numsSize > 1) ? (eng() % numsSize) : 0;

                    generateFinish = std::chrono::steady_clock::now();
                    elapsedTime = std::chrono::duration_cast < std::chrono::duration<double> >(generateFinish - generateStart).count();
                    if (elapsedTime > 3.0){
                        i = 9;
                        j = 9;
                        break;
                    }
                    
                    if (validPlacement(board, i + j*9, nums[randomIndex])){
                        board[i + j*9] = nums[randomIndex];
                        removeElement(nums, numsSize, randomIndex);
                        numsSize--;
                        break;
                    }

                    count++;

                }
                while(count < 100); 

                // If it got stuck, retry that row. 
                // This is a really terrible method and relies on luck, but eventually it works. 
                // I am trying to make a much smarter algorithm.
                
                if (count == 100){
                    for (int i = 0 ; i < 9 ; i++){
                        board[i + j*9] = 0;
                    }
                    j--;
                    count = 0;
                    break;
                }
            }

            numsSize = 9;
            delete[] nums;
            nums = nullptr;
            nums = new int[9]{1,2,3,4,5,6,7,8,9};
        }

        // Checking to see if the board is complete, if not, the loop will restart and redo the whole board
        if (board[80] != 0){
            break;
        }
    }

    return board;
}

bool validRow(int * board, int row){
    for (int i = 0 ; i < 9 ; i++){
        //std::cout << "valid row" << i << std::endl;
        if (board[row*9 + i] != 0){
            if (!validPlacement(board, row*9 + i, board[row*9+i])){
                return false;
            }
        }
        std::cout << "valid row" << i << " board[i]: " << board[row*9+i] << std::endl;
    }
    return true;
}

void printArray(int * array, int size){
    for (int i = 0 ; i < size ; i++){
        std::cout << "array[" << i << "]: " << array[i] << std::endl;
    }
}

bool noDuplicates(int* array, int size){
    for (int i = 0 ; i < size - 1 ; i++){
        for (int j = i + 1 ; j < size ; j++){

            //if (i != j){
                if (array[i] == array[j]){
                    return false;
                }
            //}
        }
    }
    return true;
}

// This function is used when genrating a new puzzle. 
// It determines if a number can be placed at a certain point on the board without breaking the rules of sudoku.
bool validPlacement(int * board, int choice, int value){
    int * array = new int [0];
    int size = 0;

    // std::cout << "" << std::endl;
    // std::cout << "" << std::endl;


    int topElementInCol = 0;
    /////////////////////////////////////////////////////////////////////////
    // Columns checking
    for (int i = choice ; i >= 0 ; i -= 9){
        
        if (i < 9){
            topElementInCol = i;
            break;
        }
    }

    for (int i = topElementInCol ; i < 81 ; i += 9){
        if (i == choice){
            //if (board[i] != 0){
                addElement(array, value, size);
                size++;
            //}
        }
        else{
            if (board[i] != 0){
                addElement(array, board[i], size);
                size++;
            }
        }
    }

    // for (int i = 0 ; i < size ; i++){
    //     std::cout << "column check array[" << i << "]: " << array[i] << std::endl;
    // }

    if (!noDuplicates(array, size)){
        return false;
    }

    delete[] array;
    array = nullptr;
    array = new int[0];
    size = 0;

    /////////////////////////////////////////////////////////////////////////////////
    // Rows checking
    for (int i = (choice - (choice % 9)) ; i < (choice - (choice % 9) + 9) ; i++){
        if (i == choice){
            //if (board[i] != 0){
                addElement(array, value, size);
                size++;
            //}
        }
        else{
            if (board[i] != 0){
                addElement(array, board[i], size);
                size++;
            }
        }
    }

    // for (int i = 0 ; i < size ; i++){
    //     std::cout << "rows check array[i]: " << array[i] << std::endl;
    // }

    if (!noDuplicates(array, size)){
        return false;
    }

    delete[] array;
    array = nullptr;
    array = new int[0];
    size = 0;

    ////////////////////////////////////////////////////////////////////
    // Boxes checking
    int row = 0;
    int col = 0;
    
    if ((choice % 9) < 3){
        col = 0;
    }
    else if ((choice % 9) < 6){
        col = 1;
    }
    else if ((choice % 9) < 9){
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
        if ((topLeftNum + i + increment) == choice){
            //if (board[topLeftNum + i + increment] != 0){
                addElement(array, value, size);
                size++;
            //}
        }
        else{
            if (board[topLeftNum + i + increment] != 0){
                addElement(array, board[topLeftNum + i + increment], size);
                size++;
            }
        }
        //std::cout << "Box index: " << topLeftNum + i + increment << "  board[i] = " << board[topLeftNum + i + increment] << " i: " << i << std::endl;
        // array[(board[topLeftNum + i + increment] & 0b00001111) - 1] = (board[topLeftNum + i + increment] & 0b00001111);
        // std::cout << "index: " << (board[topLeftNum + i + increment] & 0b00001111) - 1 << "  value: " << (board[topLeftNum + i + increment] & 0b00001111) << std::endl;
        if (i == 2 && increment < 18){
            i = -1; // Needs to go back to -1, not 0 because 1 will be added when the for loop repeats directly after this, making it 0
            increment += 9;
        }
    }

    // for (int i = 0 ; i < size ; i++){
    //     std::cout << "box check array[i]: " << array[i] << std::endl;
    // }
    //printArray(array, size);

    if (!noDuplicates(array, size)){
        return false;
    }

    delete[] array;
    array = nullptr;
    array = new int[0];
    size = 0;

    return true;
}

bool isCorrectGuess(int * board, int choice, int value){
    if ( (board[choice] & 0b00001111) == value){
        return true;
    }
    return false;
}

int addElement(int *& array, int num, int size){
    int * newArray = new int[size + 1];

    for (int i = 0 ; i < size ; i++){
        newArray[i] = array[i];
    }
    newArray[size] = num;

    //std::cout << "add done" << std::endl;
    // Sorting
    int temp = 0;
    for (int i = 0 ; i < size + 1 ; i++){

        for (int j = i ; j < size + 1 ; j++){

            if (newArray[i] > newArray[j]){
                temp = newArray[i];
                newArray[i] = newArray[j];
                newArray[j] = temp;
            }
        }
    }

    //std::cout << "done" << std::endl;
    // Deleting
    delete[] array;
    array = nullptr;

    array = newArray;
    return size + 1;
}

void removeElement(int *& array, int size, int index){
    if (size == 1){
        delete[] array;
        array = nullptr;
        return;
    }
    int * newArray = new int[size-1];

    for (int i = 0 ; i < index ; i++){
        newArray[i] = array[i];
    }
    for (int i = index + 1 ; i < size ; i++){
        newArray[i - 1] = array[i];
    }

    delete[] array;
    array = nullptr;

    array = newArray;
}

bool isGameWon(int * board){

    for (int i = 0 ; i < 81 ; i++){
        if ((board[i] & 0b00010000) != 0){
            return false;
        }
    }

    return true;
}

void unhide(int * board, int index, int *& usageCount){
    board[index] = board[index] ^ 0b00010000;
    usageCount[board[index] - 1]++;
}

int userInput(int * board, int lives, int *& usageCount){
    int x = 0;
    int y = 0;
    bool valid = false;

    do
    {
        system("cls");
        printBoard(board, lives, usageCount);

        std::cout << "Please input your x coordinate: " << std::endl;
        std::cin >> x;

        std::cout << "Please input your y coordinate: " << std::endl;
        std::cin >> y;

        valid = (x < 10 && x > 0) && (y < 10 && y > 0) && (!checkIfHidden(board, x, y));

        while (valid){
            system("cls");
            printBoard(board, lives, usageCount);

            std::cout << "Sorry that tile is invalid, or has already been revealed!" << std::endl;
            std::cout << "Please try again..." << std::endl;

            std::cout << "Please input your x coordinate: " << std::endl;
            std::cin >> x;

            std::cout << "Please input your y coordinate: " << std::endl;
            std::cin >> y;
            valid = (x < 10 && x > 0) && (y < 10 && y > 0) && (!checkIfHidden(board, x, y));
        }
    }
    while (valid);
    
    return (x + (y-1)*9 - 1);
}

bool checkIfHidden(int * board, int x, int y){
    return ( (board[x + (y-1)*9 - 1 ] & 0b00010000) == 0b00010000); // Returns true if hidden, false if not
}

void printBoard(int * board, int lives, int *& usageCount){

    int hiddenMask = 0b00010000; // Bit to check if the board value at a specific loction is hidden or not
    int count = 0;
    int columnCount = 1;

    std::cout << "    1  2  3   4  5  6   7  8  9" << "     Lives Left: " << lives << std::endl;
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
        if (j <= 9 && j >= 1){
            std::cout << "   " << j << "'s: " << usageCount[j-1];
            if ( usageCount[j-1] == 9){
                std::cout << "  done";
            }
        }
        std::cout << std::endl;
    }
}