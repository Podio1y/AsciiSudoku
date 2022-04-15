#include <iostream>
#include <chrono>
#include <random>

int main();
void printBoard(int*);
bool checkIfHidden(int*, int, int);
int userInput(int*);
void unhide(int*, int);
bool isGameWon(int*);
bool isCorrectGuess(int*, int, int);
bool noDuplicatesOrZero(int*);
int * generateBoard();
void removeElement(int*&, int, int);
int addElement(int*&, int, int);
bool validPlacement(int*, int, int);
void printArray(int*, int);
void addBox(int*&, int, int*);
int * newGen();
void removeElementByValue(int*&, int, int);

// Each row you can make arrays that show where each number fits. i.e 5 fits into these slots {1,3,5,6,7}
// or 8 fits into these slots {0,2,3}. 
// Then run an algorithm o find a combination which fills all 9, only using one each. DONE  b8

// After making first row randomly:
// Check what each box and column already contains. 
// If a column is in a box, and they share a common needed value, that should work. 
// Also check if that common needed value is elsewhere as well, to make sure not to block another spot.

int main(){

    // int * board = new int [81]{5,9,1,4,2,3,7,6,8,4,2,8,6,7,5,1,3,9,3,7,6,9,8,1,2,4,5,6,8,7,3,5,4,9,2,1,9,5,3,2,1,6,4,8,7,2,1,4,8,9,7,6,5,3,1,6,2,5,3,9,8,7,4,8,3,9,7,4,2,5,1,6,7,4,5,1,6,8,3,9,2}; // Board array
    int * board = new int [81];
    // int * testBoard = new int[81]{5,9,0,4,2,3,7,6,8,4,2,8,6,7,5,1,3,9,3,7,6,9,8,1,2,4,5,6,8,7,3,5,4,9,2,1,9,5,3,2,1,6,4,8,7,2,1,4,8,9,7,6,5,3,1,6,2,5,3,9,8,7,4,8,3,9,7,4,2,5,1,6,7,4,5,1,6,8,3,9,2};
    int choice = 0;
    int lives = 3;
    int value = 0;

    for (int i = 0 ; i < 81 ; i++){
        board[i] = 0;
    }

    std::cout << "Generating Board..." <<std::endl;
    board = generateBoard();
    //board = newGen();

    // TESING validPlacement function
    // board[10] = 1;
    // board[11] = 3;
    // board[1] = 4;
    // std::cout << validPlacement(board, 0, 2);

    // For loop for testing the printBoard funtion. Tests to see if printing hidden/unhidden values properly
    // for (int i = 0 ; i < 81 ; i++){
    //     //board[i] = (i % 9) + 1;
    //     if (i % 9 != 0){
    //         board[i] = board[i] ^ 0b00010000;
    //     }
    // }
    printBoard(board);

    // TEST ISCORRECTGUESS() WITH THE ZEROES AND UN-INITIALIZED VALUES TO SEE IF IT WORKS
    // std::cout << "test";
    // board = generateBoard();
    // printBoard(board);

    // while(lives > 0){
    //     system("cls");
    //     printBoard(board);

    //     choice = userInput(board);
        
    //     std::cout << "Please enter your guess: " << std::endl;
    //     std::cin >> value;

    //     while ((!isCorrectGuess(board, choice, value) && lives > 0)){
    //         std::cout << "That can't go there!" << std::endl;
    //         lives--;
    //         choice = userInput(board);

    //         std::cout << "Please enter your guess: " << std::endl;
    //         std::cin >> value;
    //     }
    //     if (lives <= 0 || isGameWon(board)){
    //         break;
    //     }
    //     unhide(board, choice);
    // }
    // std::cout << checkIfHidden(board, 2, 8);

    delete board;
    board = nullptr;

    return 0;
}

int * newGen(){
    // Random seed for the modern random engine
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();

    // Random engine
    std::default_random_engine eng(seed);

    int * board = new int [81];

    for (int i = 9 ; i < 81 ; i++){
        board[i] = 0;
    }

    bool numFound = false;
    int randomNum = 0;

    int * options = new int [9]{1,2,3,4,5,6,7,8,9};
    int optionsSize = 9;

    for (int i = 0 ; i < 9 ; i++){
        board[i] = i + 1;
    }

    for (int i = 9 ; i < 81 ; i++){

        // do{
        //     randomNum = (eng() % 9) + 1;
        //     numFound = validPlacement(board, i, randomNum);
        //     board[i] = randomNum;
        //     std::cout << randomNum << " - " << numFound << std::endl;
        //     //printBoard(board);
        // }
        // while(!numFound);
        // std::cout << "============  " << i << std::endl;
        // printBoard(board);

        do{
            randomNum = eng() % optionsSize;
            numFound = validPlacement(board, i, options[randomNum]);
            board[i] = options[randomNum];
            //printArray(options, optionsSize);
            removeElement(options, optionsSize, randomNum);
            optionsSize--;
        }
        while(!numFound && optionsSize > 0);
        if ((optionsSize == 0) && (((i - 1) % 9) != 8)){
            i -= i%9;
        }
        std::cout << i << std::endl;
        printBoard(board);
        delete[] options;
        options = nullptr;
        options = new int[9]{1,2,3,4,5,6,7,8,9};
        optionsSize = 9;
    }

    return board;
}

void removeElementByValue(int * array, int element, int size){
    for (int i = 0 ; i < size ; i++){
        if (array[i] == element){
            removeElement(array, size, i);
        }
    }
}

int* optimalPlacements(int** arrays, int size){
    int* optimalPlaces = new int[9];


}

void addBox(int*& board, int boxnum, int* box){
    int topLeftNum = 0;

    for (int i = 0 ; i < 3 ; i++){
        if (boxnum <= (3*(i + 1)) ){
            topLeftNum = (i * 27) + (boxnum - (1 + i*3))*3;
            break;
        }
    }


    int increment = 0;
    int index = 0;

    for (int i = 0 ; i < 3 ; i++){
        board[topLeftNum + i + increment] = box[index];
        if (i == 2 && increment < 18){
            i = -1; // Needs to go back to -1, not 0 because 1 will be added when the for loop repeats directly after this, making it 0
            increment += 9;
        }
        index++;
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

    for (int i = 0 ; i < 81 ; i++){
        board[i] = 0;
    }

    // int * box1 = new int[9]{0,0,0,0,0,0,0,0,0};
    // int * box2 = new int[9]{0,0,0,0,0,0,0,0,0};
    // int * box3 = new int[9]{0,0,0,0,0,0,0,0,0};
    // int * box4 = new int[9]{0,0,0,0,0,0,0,0,0};
    // int * box5 = new int[9]{0,0,0,0,0,0,0,0,0};
    // int * box6 = new int[9]{0,0,0,0,0,0,0,0,0};
    // int * box7 = new int[9]{0,0,0,0,0,0,0,0,0};
    // int * box8 = new int[9]{0,0,0,0,0,0,0,0,0};
    // int * box9 = new int[9]{0,0,0,0,0,0,0,0,0};

    // int * col1 = new int[9]{0,1,2,3,4,5,6,7,8};
    // int * row1 = new int[9]{0,1,2,3,4,5,6,7,8};

    // int * col2 = new int[9]{0,1,2,3,4,5,6,7,8};
    // int * row2 = new int[9]{0,1,2,3,4,5,6,7,8};

    // int * col3 = new int[9]{0,1,2,3,4,5,6,7,8};
    // int * row3 = new int[9]{0,1,2,3,4,5,6,7,8};

    // int * col4 = new int[9]{0,1,2,3,4,5,6,7,8};
    // int * row4 = new int[9]{0,1,2,3,4,5,6,7,8};

    // int * col5 = new int[9]{0,1,2,3,4,5,6,7,8};
    // int * row5 = new int[9]{0,1,2,3,4,5,6,7,8};

    // int * col6 = new int[9]{0,1,2,3,4,5,6,7,8};
    // int * row6 = new int[9]{0,1,2,3,4,5,6,7,8};

    // int * col7 = new int[9]{0,1,2,3,4,5,6,7,8};
    // int * row7 = new int[9]{0,1,2,3,4,5,6,7,8};

    // int * col8 = new int[9]{0,1,2,3,4,5,6,7,8};
    // int * row8 = new int[9]{0,1,2,3,4,5,6,7,8};

    // int * col9 = new int[9]{0,1,2,3,4,5,6,7,8};
    // int * row9 = new int[9]{0,1,2,3,4,5,6,7,8};

    // int ** boxes = new int * [9]{box1, box2, box3, box4, box5, box6, box7, box8, box9};
    // int ** cols = new int * [9]{col1, col2, col3, col4, col5, col6, col7, col8, col9};
    // int ** rows = new int * [9]{row1, row2, row3, row4, row5, row6, row7, row8, row9};

    // int randomRow = 0;
    // int randomCol = 0;
    // int * colandrownum = new int[9]{0,1,2,3,4,5,6,7,8};

    // for (int i = 0 ; i < 9 ; i++){

    //     for (int j = 0 ; j < 9 ; j++){
    //         randomRow = eng() % 3 + (j/3)*3;
    //         randomCol = eng() % 3 + (j/3)*3;
    //         //boxes[i][j] = j + 1; rows[i][random]
    //     }
    // }

        int count = 0;
        int count35 = 0;
        int randomIndex = 0;
        
        for (int j = 0 ; j < 9 ; j++){
            for (int i = 0 ; i < 9 ; i++){
                

                do {
                    randomIndex = (numsSize > 1) ? (eng() % numsSize) : 0;
                    //std::cout << "potential guess: " << nums[randomIndex] << std::endl;
                    if (validPlacement(board, i + j*9, nums[randomIndex])){
                        board[i + j*9] = nums[randomIndex];
                        removeElement(nums, numsSize, randomIndex);
                        numsSize--;
                        break;
                    }
                    //printArray(nums, numsSize);
                    //printBoard(board);
                    //std::cout << i << j << "leaking" << count << std::endl;
                    count += 5;
                    // if (i == 3 && j == 5){
                    //     count35 += 5;
                    // }
                }
                while(count < 500); 

                // If it got stuck, retry that row. 
                // This is a really terrible method and relies on luck, but eventually it works. 
                // I am trying to make a much smarter algorithm.
                if (count == 500){
                    for (int i = 0 ; i < 9 ; i++){
                        board[i + j*9] = 0;
                    }
                    j--;
                    count = 0;
                    break;
                }
            }
            //if (count35 >= 3500){
            //    break;
            //}
            numsSize = 9;
            delete[] nums;
            nums = nullptr;
            nums = new int[9]{1,2,3,4,5,6,7,8,9};
        }
    
    return board;
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

    if (!noDuplicates(array, size)){
        return false;
    }

    delete[] array;
    array = nullptr;
    array = new int[0];
    size = 0;

    return true;
}

// ========================================================================================
// ========================================================================================

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