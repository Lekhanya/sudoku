/*
Matt Olson
Alex Izydorczyk

File to implement game play
*/

#include <iostream>
#include <regex>
#include <sstream>
#include "game.hpp"
#include "solver.hpp"
#include "altproj.hpp"
using namespace std;

void playGame(int size, int nobs);

void dispalyHelp(){
    
    cout << 
    "   1. To fill in cell, enter the column number, \n\
    row number, and value you want to enter (1 based). \n\
    Separate with spaces, commas, or any other delimiter\n\
    (besides an integer of course)\n\
    2. At any time, enter \"Solve\" to have the backtracer\n\
    solve the game for you based on your current position.\n\
    3. If you've walked yourself into an impossible configuration \n\
    you will be prompted to first clear the board\n\
    4. Once solved, you will be asked if you want to play again \n" 
    << endl;
}

//play game again
void playAgainPrompt(int size, int nobs){
    string user_response;

    do{
        cout << "Would you like to play again? y/n" << endl;
        getline(cin, user_response);
    } while ((user_response != "y") && (user_response != "n"));

    if (user_response == "y"){
        playGame(size, nobs);
    } else {
        return;
    }
}

void playGame(int size, int nobs){

    string user_entry;
    bool solverSucces = false;
    Board board = generatePuzzle(size, nobs);
    regex rgx("[0-9]{1,}"); // For parsing user input
    smatch match;

    int input_int[3]; // Array for user input

    // Repeat until puzzle is solved
    do {

        board.printPuzzle(); // print puzzle
        getline(cin, user_entry); // get user input

        // Check for user commands
        if (user_entry == "Solve" || user_entry == "solve"){
            solverSucces = solve(board,0,0);
            board.printPuzzle();
            break;
        }

        if (user_entry == "Clear" || user_entry == "clear"){
            board.clearPuzzle();
            continue;
        }

        if (user_entry == "help" || user_entry == "Help" 
            || user_entry == "h"){
                dispalyHelp();
            }

        // Assuming no special command, attempt to parse user input
        int count = 0;
        for(sregex_iterator i = sregex_iterator(user_entry.begin(), 
                                                user_entry.end(), rgx);
                                i != sregex_iterator();
                                ++i )
        {
            smatch m = *i;
            input_int[count] = stoi(m.str());
            ++count;
        }


        // If parse values are out of range... throw error and continue
        bool invalid = false;
        for (int i=0; i<3; i++) {
		    if (!board.inBounds(input_int[i])){
		    	invalid = true;
		    }
		}

		if (invalid){
			cout << "Values entered must be between 1 and " << board.getSize() << endl;
			continue;
		}

        // Assign board value if feasible move
        if (board.feasibleUser((input_int[0]-1),(input_int[1]-1),input_int[2])==true){

            if (board.checkImmutable((input_int[0]-1),(input_int[1]-1)) == false){
                board.assignValue((input_int[0]-1),(input_int[1]-1),input_int[2]);

            } else {
                cout << "You can't change that value!" << endl;
            }

        } else {
            cout << "Not feasible!" << endl;
        }

        // reset inputs to 0, will cause invalid message if unchanged on next round
		for (int i=0; i<3; i++) {
		    input_int[i] = 0;
		}

    } while (!board.checkPuzzle());

    string user_response;

    if (solverSucces){

        cout << "The puzzle is solved!" << endl;
        playAgainPrompt(size, nobs);

    } else {

        cout << "Could not solve puzzle based on pre-filled values" << endl;

        do {

            cout << "Would you like to clear and try to solve? y/n" << endl;
            getline(cin, user_response);

        } while ((user_response != "y") && (user_response != "n"));

        if (user_response == "y"){
            board.clearPuzzle();
            solve(board,0,0);
            board.printPuzzle();
            playAgainPrompt(size, nobs);
        }

    }


}
