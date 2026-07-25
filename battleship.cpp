#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

// Terminal color codes. RESET restores the default color and must be printed after every colored output.
const std::string RED    = "\033[31m";
const std::string GREEN  = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string CYAN   = "\033[36m";
const std::string RESET  = "\033[0m";

// Reads a single integer from the user.
// Returns false if the input is not a number, after clearing the stream so the caller can ask again.
// If the input stream is closed (Ctrl+D / EOF), exits instead of looping forever.
bool readInt(int &value){
    std::cin >> value;

    if(std::cin.fail()){
        if(std::cin.eof()){
            std::cout << "\nInput closed, exiting." << std::endl;
            std::exit(0);
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }

    return true;
}

// Prints the board to the screen.
// reveal = true  -> undamaged ships are shown as a cyan 'S' (used for your own board).
// reveal = false -> ships look the same as empty cells (used for the enemy board).
// Hit cells are printed as a green X, missed cells as a red X.
void printBoard(char ships[4][4], bool reveal = false){
    std::cout << "\n  0 1 2 3" << std::endl;

    for(int i = 0; i < 4; i++){
        std::cout << i << " ";

        for(int j = 0; j < 4; j++){
            char board = ships[i][j];

            if(board == '1'){
                // A ship is here: only show it on your own board
                if(reveal){
                    std::cout << CYAN << "S" << RESET << " ";
                }
                else{
                    std::cout << "0 ";
                }
            }
            else if(board == 'H'){
                std::cout << GREEN << "X" << RESET << " ";
            }
            else if(board == 'M'){
                std::cout << RED << "X" << RESET << " ";
            }
            else{
                std::cout << board << " ";
            }
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

// Single player mode.
// Ships are placed randomly and the player has 8 misses to find them all.
// Hits do not cost anything; only misses decrease attemptsRemaining.
void playSolo(){
    int row, column, attemptsRemaining = 8, attemptsUsed = 0, shipsRemaining = 4, placedShips = 0, totalShips = 4;

    char ships[4][4] = {
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'}
    };

    // Place the ships randomly. Retry if the chosen cell is already taken.
    while(placedShips < totalShips){
        int r = rand() % 4;
        int c = rand() % 4;

        if(ships[r][c] == '0'){
            ships[r][c] = '1';
            placedShips++;
        }
    }

    printBoard(ships);

    // Main game loop: runs until all ships are sunk or the attempts run out
    while(shipsRemaining > 0 && attemptsRemaining > 0){
        std::cout << "Type a row to fire (Type between 0-3): ";
        if(!readInt(row)){
            std::cout << CYAN << "Invalid input, try again!" << RESET << std::endl;
            continue;
        }

        std::cout << "Type a column to fire (Type between 0-3): ";
        if(!readInt(column)){
            std::cout << CYAN << "Invalid input, try again!" << RESET << std::endl;
            continue;
        }

        // Prevent access outside the array bounds
        if(row < 0 || row > 3 || column < 0 || column > 3){
            std::cout << CYAN << "Invalid coordinates, try again!" << RESET << std::endl;
            std::cout << "--------------------------------------------\n";
            continue;
        }

        if(ships[row][column] == 'H' || ships[row][column] == 'M'){
            // Firing at the same cell again does not cost an attempt
            std::cout << YELLOW << "You already fired that area, try again!" << RESET << std::endl;
            std::cout << "--------------------------------------------\n";
        }
        else if(ships[row][column] == '1'){
            std::cout << GREEN << "Target hit!" << RESET << std::endl;
            ships[row][column] = 'H';
            shipsRemaining = shipsRemaining - 1;
            attemptsUsed++;

            std::cout << "Ships remaining: " << shipsRemaining << std::endl;
            std::cout << "Attempts remaining: " << attemptsRemaining << std::endl;
            std::cout << "--------------------------------------------\n";
        }
        else if(ships[row][column] == '0'){
            std::cout << RED << "Target miss!" << RESET << std::endl;
            ships[row][column] = 'M';
            attemptsRemaining--;
            attemptsUsed++;

            std::cout << "Ships remaining: " << shipsRemaining << std::endl;
            std::cout << "Attempts remaining: " << attemptsRemaining << std::endl;
            std::cout << "--------------------------------------------\n";
        }

        printBoard(ships);
    }

    // End of game: pick the result message based on why the loop ended
    if(shipsRemaining == 0 && attemptsRemaining > 0){
        std::cout << "You won, congrats!" << std::endl;
        std::cout << "Attempts used: " << attemptsUsed << std::endl;
    }
    else if(attemptsRemaining == 0 && shipsRemaining > 0){
        std::cout << "You lost, good luck next time!" << std::endl;
        std::cout << "Ships remaining: " << shipsRemaining << std::endl;
        std::cout << "Attempts used: " << attemptsUsed << std::endl;
    }
}

// Player versus computer mode.
// The player places their own ships first, then the two sides take turns:
// the player fires, then the computer fires at a random cell.
// The game ends when either side loses all of its ships.
void playVsComputer(){
    int row, column, attemptsUsed = 0, shipsRemaining = 4, computerShipsRemaining = 4,
        placedShips = 0, computerPlacedShips = 0, totalShips = 4, i = 1;

    // The player's board
    char ships[4][4] = {
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'}
    };

    // The computer's board
    char computerShips[4][4] = {
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'},
        {'0', '0', '0', '0'}
    };

    // Place the computer's ships randomly
    while(computerPlacedShips < totalShips){
        int r = rand() % 4;
        int c = rand() % 4;

        if(computerShips[r][c] == '0'){
            computerShips[r][c] = '1';
            computerPlacedShips++;
        }
    }

    // The player places their ships one by one
    while(placedShips < totalShips){
        std::cout << "Type a row to place " << i << ". ship (Type between 0-3): ";
        if(!readInt(row)){
            std::cout << CYAN << "Invalid input, try again!" << RESET << std::endl;
            continue;
        }

        std::cout << "Type a column to place " << i << ". ship (Type between 0-3): ";
        if(!readInt(column)){
            std::cout << CYAN << "Invalid input, try again!" << RESET << std::endl;
            continue;
        }

        // Prevent access outside the array bounds
        if(row < 0 || row > 3 || column < 0 || column > 3){
            std::cout << CYAN << "Invalid coordinates, try again!" << RESET << std::endl;
            std::cout << "--------------------------------------------\n";
            continue;
        }

        if(ships[row][column] == '1'){
            std::cout << YELLOW << "You already placed that area, try again!" << RESET << std::endl;
            std::cout << "--------------------------------------------\n";
            continue;
        }
        else{
            ships[row][column] = '1';
            std::cout << GREEN << "Ship successfully placed!" << RESET << std::endl;
            std::cout << "--------------------------------------------\n";
            placedShips++;
            i++;
        }
    }

    // Your own board: the ships should be visible
    printBoard(ships, true);

    // Main game loop: runs until one side loses all of its ships
    while(computerShipsRemaining > 0 && shipsRemaining > 0){
        std::cout << "Type a row to fire (Type between 0-3): ";
        if(!readInt(row)){
            std::cout << CYAN << "Invalid input, try again!" << RESET << std::endl;
            continue;
        }

        std::cout << "Type a column to fire (Type between 0-3): ";
        if(!readInt(column)){
            std::cout << CYAN << "Invalid input, try again!" << RESET << std::endl;
            continue;
        }

        // Prevent access outside the array bounds
        if(row < 0 || row > 3 || column < 0 || column > 3){
            std::cout << CYAN << "Invalid coordinates, try again!" << RESET << std::endl;
            std::cout << "--------------------------------------------\n";
            continue;
        }

        if(computerShips[row][column] == 'H' || computerShips[row][column] == 'M'){
            // Repeated shot: the turn does not pass to the computer
            std::cout << YELLOW << "\nYou already fired that area, try again!" << RESET << std::endl;
            std::cout << "--------------------------------------------\n";
            continue;
        }
        else if(computerShips[row][column] == '1'){
            std::cout << GREEN << "Target hit!" << RESET << std::endl;
            computerShips[row][column] = 'H';
            computerShipsRemaining = computerShipsRemaining - 1;

            std::cout << "Ships remaining: " << computerShipsRemaining << std::endl;
            std::cout << "--------------------------------------------\n";
            attemptsUsed++;
        }
        else if(computerShips[row][column] == '0'){
            std::cout << RED << "Target miss!" << RESET << std::endl;
            computerShips[row][column] = 'M';

            std::cout << "Ships remaining: " << computerShipsRemaining << std::endl;
            std::cout << "--------------------------------------------\n";
            attemptsUsed++;
        }

        // The enemy board: the ships must stay hidden
        printBoard(computerShips);

        // If the player has won, the computer should not get another turn
        if(computerShipsRemaining == 0) break;

        // The computer's turn
        if(shipsRemaining > 0){
            int shipr, shipc;

            // Keep picking until an untouched cell is found
            do{
                shipr = rand() % 4;
                shipc = rand() % 4;
            } while(ships[shipr][shipc] == 'H' || ships[shipr][shipc] == 'M');

            if(ships[shipr][shipc] == '1'){
                std::cout << RED << "Computer hit your ship at (" << shipr << "," << shipc << ")!" << RESET << std::endl;
                ships[shipr][shipc] = 'H';
                shipsRemaining--;
            }
            else{
                std::cout << GREEN << "Computer missed at (" << shipr << "," << shipc << ")." << RESET << std::endl;
                ships[shipr][shipc] = 'M';
            }

            // Your own board: the ships are visible
            printBoard(ships, true);
        }
    }

    // End of game
    if(computerShipsRemaining == 0){
        std::cout << "You won, congrats!" << std::endl;
        std::cout << "Your ships remaining: " << shipsRemaining << std::endl;
        std::cout << "Attempts used: " << attemptsUsed << std::endl;
    }
    else if(shipsRemaining == 0){
        std::cout << "You lost, good luck next time!" << std::endl;
        std::cout << "Computer ships remaining: " << computerShipsRemaining << std::endl;
        std::cout << "Attempts used: " << attemptsUsed << std::endl;
    }
}

// Shows the menu and starts the selected mode.
int main(){
    srand(time(0));   // seed the generator so the layout differs on every run

    int choice;

    std::cout << "--------------------------------------------\n";
    std::cout << "Welcome to Battleship\n1) Solo Mode\n2) Vs Computer Mode\nChoice: ";
    std::cin >> choice;
    std::cout << "--------------------------------------------" << std::endl;

    if(choice == 1){
        playSolo();
    }
    else if(choice == 2){
        playVsComputer();
    }
    else{
        std::cout << "Invalid choice!";
    }

    return 0;
}