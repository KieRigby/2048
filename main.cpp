/**
 * 
 * Software Engineering I - Introduction to Computing - Spring Coursework - 2048
 * Kieran Rigby - 01067343
 *
 */

 //include necessary files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>

//Preprocessor Constants
 #define ROWS 4
 #define COLS 4

//use standard namespace
using namespace std;

//function declarations
void initializeGameBoard(int board[][COLS], ifstream &config, bool board_from_file = false);
void printGameBoard(int board[][COLS]);
bool updateGameBoard(int board[][COLS], string move);
void insertNumber(int board[][COLS]);
bool checkGame(int board[][COLS]);
bool fullBoard(int board[][COLS]);
bool validMove(int board[][COLS], string move);

//main function
int main(){

	//string to display logo
	string logo = "  _____     ____    _    _      ____    \n / ___ `. .'    '. | |  | |   .' __ '.  \n|_/___) ||  .--.  || |__| |_  | (__) |  \n .'____.'| |    | ||____   _| .`____'.  \n/ /_____ |  `--'  |    _| |_ | (____) | \n|_______| '.____.'    |_____|`.______.' ";

	//setup an array of integers for the gameboard;
	int gameboard[ROWS][COLS];

	//setup a variable for the main game loop
	bool has_lost = false; 

	//temporary character for players move;
	string player_move;

	//variables for config file name and file stream
	string config_filename;
	ifstream config_file;

	cout << logo << endl << "By Kieran Rigby" <<  endl << endl;

	//ask the user to input a config file name.
	cout << "Enter inital config file name (enter none for no config):" << endl;
	cin >> config_filename;
	cout << endl;

	//try to open the file
	config_file.open(config_filename.c_str());

	//if file exists then set up the gameboard with those values
	if(config_file.is_open()){
		initializeGameBoard(gameboard, config_file, true);
	} else { //otherwise use a default configuration
		cout << "Couldn't find configuration file, using the default configuration." << endl;
		initializeGameBoard(gameboard, config_file);
	}

	//print the gameboard for the first time
	printGameBoard(gameboard);

	//while the user hasn't lost
	while(!has_lost){
		
		//check if any moves can be made
		if(checkGame(gameboard)){
			//if no moves can be made then if the board isn't full
			if(!fullBoard(gameboard)){
				//insert a number
				insertNumber(gameboard);	
			}else{
				//if no more moves can be made and the gameboard is full then the game is over so set the variable to true
				has_lost = true;
			}
		} else{ //if moves can be made
			//get the next move from the user
			cin >> player_move; 
			//if the move is a valid move
			if (updateGameBoard(gameboard, player_move)){ 
				//if the gameboard isn't full then
				if(!fullBoard(gameboard)){
					//insert a number
					insertNumber(gameboard);
				}
				//print the gameboard
				printGameBoard(gameboard);
			}
		}



	}


	//tell the user the game is over and thank them for playing
	cout << "Game over! Thanks for playing!" << endl;

	//return from the main function
	return 0;
}

/**
 * @brief Initialize the game board
 * @details The game board can be initialized from either a default config or a file config. This function assumes default config.
 * 
 * 
 * @param board - The gameboard array
 * @param config_file - The fstream for the config file if any.
 * @param board_from_file - If you want a gameboard from file then set this true.
 */
void initializeGameBoard(int board[][COLS], ifstream &config_file, bool board_from_file){
	if(board_from_file){
		//declare a temporary integer to read from file
		int tmp_int;

		//nested for loops go through each element of the gameboard array
		for(int i = 0; i < ROWS; i++){
			for(int j = 0; j < COLS; j++){
				//read the current number from the file
				config_file >> tmp_int;
				//put it in the current position on the gameboard
				board[i][j] = tmp_int;
			}
		}

	}else{
		//nested for loops go through each element of the gameboard array
		for(int i = 0; i < ROWS; i++){
			for(int j = 0; j < COLS; j++){
				//if the current element is the bottom right then set current element to 2
				if (i == ROWS - 1 && j == COLS - 1){
					board[i][j] = 2;
				} else{ //if the current element is the bottom right then set current element to 0.
					board[i][j] = 0;
				}
			}
		}
	}
}

/**
 * @brief Print out the game board.
 * @details This will format the game board in a nice manner for the console and print out each element onto the screen.
 * 
 * @param board - The game board to print out.
 */
void printGameBoard(int board[][COLS]){
	//print out an initial line for the start of the box
	cout << "  ";
	for (int i = 0; i <= COLS*6; i++){
			cout << "-";
		} 
	cout << endl;
	//for each row
	for(int i = 0; i < ROWS; i++){
		//print out a starting line
		cout << "  |";
		//for each column in the row
		for (int j = 0; j < COLS; j++){
			//check how many digits the number is and add an appropriate amount of spaces
			if(board[i][j] < 10){
				cout << board[i][j] << "    |";
			} else if(board[i][j] < 100){
				cout << board[i][j] << "   |";
			} else if (board[i][j] < 1000){
				cout << board[i][j] << "  |";
			} else if (board[i][j] < 10000){
				cout << board[i][j] << " |";
			} else if (board[i][j] < 100000){
				cout << board[i][j] << "|";
			}
			
		}
		//for  each row add a new line break
		cout << endl << "  ";
		for (int i = 0; i <= COLS*6; i++){
			cout << "-";
		} 
		cout << endl;
	}
	//skip a line at the end of the box
	cout << endl;
}

/**
 * @brief Updates the game board given a move
 * @details This will update the game board passed into the function based upon the move also passed into the function
 * 
 * @param board - An array containing the gameboard
 * @param move - A character of the move to be executed.
 * @return boolean - returns true if the board has updated successfully, returns false if the move supplied is invalid
 */
bool updateGameBoard(int board[][COLS], string move){
	//check if a move can be made
	if(validMove(board, move)){
		//if the move is upwards
		if (move == "w"){
			//first for loop is for shifting all the elements down
			//for each element on the gameboard apart from the top row
			for(int i = 1; i < ROWS; i++){
				for(int j = 0; j < COLS; j++){
					// if the element isn't 0 then 
					if(board[i][j] != 0){
						// for every element above the current element starting from the top
						for (int k = 0; k < i; k++){
							//if the element is 0 then
							if (board[k][j] == 0){
								//move the current element to the position of the 0 element above it
								board[k][j] = board[i][j];
								board[i][j] = 0;
							}
						}
					}
				}
			}

			//second for loop is for combining numbers
			//for each element in the array apart from the top row
			for(int i = 1; i < ROWS; i++){
				for (int j = 0; j < COLS; j++){
					// if the current element is the same as the one above it
					if (board[i][j] == board[i-1][j]){
						//combine the two
						board[i-1][j] = board[i][j] * 2;
						//for all other elements below it
						for (int k = i; k < ROWS; k++){
							//if it is the bottom row
							if (k == ROWS - 1){
								//add a zero to the bottom row
								board[k][j] = 0;
							}else{ 
								//otherwise shift the other elements to the position above
								board[k][j] = board[k+1][j];
								board[k+1][j] = 0;
							}
						}
					}
				}
			}

		return true;
		} else if (move == "a"){

			//first for loop is for shifting all the elements down
			//for each element on the gameboard apart from the left column
			for(int i = 0; i < ROWS; i++){
				for(int j = 1; j < COLS; j++){
					// if the element is 0 then move to the next one
					if(board[i][j] != 0){
						// for every element to the right of the current element starting from the left
						for (int k = 0; k < j; k++){
							//if the element is 0 then
							if (board[i][k] == 0){
								//move the current element to the position of the 0 element to the left of it
								board[i][k] = board[i][j];
								board[i][j] = 0;
							}
						}
					}
				}
			}

			//second for loop is for combining numbers
			//for each element in the array apart from the top row
			for(int i = 0; i < ROWS; i++){
				for (int j = 1; j < COLS; j++){
					//if the current element is equal to the element on the left
					if (board[i][j] == board[i][j-1]){
						//combine the elements
						board[i][j-1] = board[i][j] * 2;
						//for each element to the right of current element
						for (int k = j; k < COLS; k++){
							//if the far right element
							if (k == COLS - 1){
								//set equal to 0
								board[i][k] = 0;
							}else{
								//if not the far right element set equal to the element on the right
								board[i][k] = board[i][k+1];
								//and set the element on the right equal to 0
								board[i][k+1] = 0;
							}
						}
					}
				}
			}

			return true;
		} else if (move == "s"){
			//first for loop is for shifting all the elements down
			//for each element on the gameboard apart from the bottom row
			for(int i = ROWS-2; i >=0; i--){
				for(int j = 0; j < COLS; j++){
					// if the element isn't 0 then 
					if(board[i][j] != 0){
						// for every element below the current element starting from the bottom
						for (int k = ROWS-1; k >= i; k--){
							//if the element is 0 then
							if (board[k][j] == 0){
								//move the current element to the position of the 0 element below it
								board[k][j] = board[i][j];
								board[i][j] = 0;
							}
						}
					}
				}
			}

			//second for loop is for combining numbers
			//for each element in the array apart from the bottom row
			for(int i = ROWS-2; i >= 0; i--){
				for (int j = 0; j < COLS; j++){
					// if the current element is the same as the one below it
					if (board[i][j] == board[i+1][j]){
						//combine the two
						board[i+1][j] = board[i][j] * 2;
						//for all other elements above it
						for (int k = i; k >= 0; k--){
							//if it is the top row
							if (k == 0){
								//add a zero to the top row
								board[k][j] = 0;
							}else{ 
								//otherwise shift the other elements to the position below
								board[k][j] = board[k-1][j];
								board[k-1][j] = 0;
							}
						}
					}
				}
			}

			return true;
		} else if (move == "d"){
			//first for loop is for shifting all the elements down
			//for each element on the gameboard apart from the right column
			for(int i = 0; i < ROWS; i++){
				for(int j = COLS - 2; j >= 0; j--){
					// if the element is 0 then move to the next one
					if(board[i][j] != 0){
						// for every element to the right of the current element starting from the right
						for (int k = COLS-1; k > j; k--){
							//if the element is 0 then
							if (board[i][k] == 0){
								//move the current element to the position of the 0 element to the right of it
								board[i][k] = board[i][j];
								board[i][j] = 0;
							}
						}
					}
				}
			}

			//second for loop is for combining numbers
			//for each element in the array apart from the right column
			for(int i = 0; i < ROWS; i++){
				for (int j = COLS-2; j >= 0; j--){
					//if the current element is equal to the element on the right
					if (board[i][j] == board[i][j+1]){
						//combine the elements
						board[i][j+1] = board[i][j] * 2;
						//for each element to the left of current element
						for (int k = j; k >= 0; k--){
							//if the far left element
							if (k == 0){
								//set equal to 0
								board[i][k] = 0;
							}else{
								//if not the far left element set equal to the element on the left
								board[i][k] = board[i][k-1];
								//and set the element on the left equal to 0
								board[i][k-1] = 0;
							}
						}
					}
				}
			}
			return true;
		} else{
			return false;
		}
	}else{
		return false;
	}
}

/**
 * @brief Inserts a number randomly on the gameboard
 * @details Uses a pseudo-random number generator and tests the positions on a gameboard to put the new number in an empty location
 * 
 * @param board - An array containing the gameboard
 */
void insertNumber(int board[][COLS]){
	//give the random number generator a random seed;
	srand (time(NULL));
	
	//declare two vectors to store row and column index of free rows and columns
	vector<int> freeRowIndex;
	vector<int> freeColIndex;

	//for each element in the 2D array
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			// if it is empty
			if(board[i][j] == 0){
				//store the index of the row and column onto the two vectors
				freeRowIndex.push_back(i);
				freeColIndex.push_back(j);
			}
		}
	}

	//generate a random number between 0 and the size of vectors - 1
	int randIndex = rand() % freeRowIndex.size();
	//set this random free spot equal to 2.
	board[freeRowIndex[randIndex]][freeColIndex[randIndex]] = 2;
}

/**
 * @brief Check to see if the game has ended or not
 * @details Checks all the cells on the game board and compares them with the cell next to them to check if the numbers match or not
 * 
 * @param board - The gameboard to check
 * @return Boolean: True if game is ended false if game is still in progress.
 */
bool checkGame(int board[][COLS]){
	//assume that the rows and cols have no possible moves
	bool ROW_CHECK = true;
	bool COL_CHECK = true;

	//for each vertical strip
	for(int i = 0; i < ROWS - 1; i++){
		for(int j = 0; j < COLS; j++){
			//if any of the elements are 0 or are next to another element of the same value then
			if (board[i][j] == board[i+1][j] || board[i][j] == 0 || board[i+1][j] == 0){
				//there are possible moves in the rows
				ROW_CHECK = false;
			}
		}
	}

	//for each horizontal strip
	for(int k = 0; k < COLS - 1; k++){
		for(int l = 0; l < ROWS; l++){
			//if any of the elements are 0 or are next to another element of the same value then
			if (board[l][k] == board[l][k+1] || board[l][k] == 0 || board[l][k+1] == 0){
				//there are possible moves in the cols
				COL_CHECK = false;
			}
		}
	}

	//return if both are true 
	return ROW_CHECK && COL_CHECK;

}

/**
 * @brief Checks for a full game board
 * @details This function takes a gameboard array and checks if the gameboard is full, returns true if full, false if not
 * 
 * @param board Gameboard array of integers
 * @return boolean - true for full board / false for not full gameboard
 */
bool fullBoard(int board[][COLS]){
	//for each element in the gameboard
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			//if the element is 0 then 
			if (board[i][j] == 0){
				//the gameboard isn't full so return false
				return false;
			}
		}
	}
	//no 0 elements have been found so return true;
	return true;
}

/**
 * @brief Check if a move is valid in a given direction
 * @details Checks to see if
 * 
 * @param board - The gameboard to be checked
 * @param move - The move that the player wants to make
 * 
 * @return boolean - true if the move is valid, false if the move is not valid;
 */
bool validMove(int board[][COLS], string move){
	//assume that the move is not valid
	bool check = false;
	//if the player is moving up
	if (move == "w"){
		//for each vertical strip
		for(int i =0; i < COLS; i++){
			//start from the top and go to the pen-ultimate row
			for(int j = 0; j < ROWS - 1; j++){
				//if the current element is not 0 then
				if(board[j][i] != 0){
					//if the current element is the same as the one below it
					if(board[j][i] == board[j+1][i]){
						//the move is valid
						check = true;
					}
				}else{ //if the current element is 0 then
					//if the element below the current one isn't 0 then 
					if(board[j+1][i] != 0){
						//the move is valid
						check = true;
					}
				}
			}
		}
	//if the player is moving left
	}else if(move == "a"){
		//for each horizontal strip
		for(int i =0; i < ROWS; i++){
			//start from the left and go to the element second from the right
			for(int j = 0; j < COLS - 1; j++){
				//if the current element is not zero
				if(board[i][j] != 0){
					//if the current element as the same as the element on it's right then
					if(board[i][j] == board[i][j+1]){
						//the move is valid
						check = true;
					}
				}else{ //if the current element is zero then
					//if the element to the right isn't 0 then
					if(board[i][j+1] != 0){
						//the move is valid;
						check = true;
					}
				}
			}
		}
	//if the player is moving downwards
	}else if(move == "s"){
		//for each vertical strip
		for(int i = 0; i < COLS; i++){
			//starting from the bottom and going to the element second from the top
			for(int j = ROWS - 1; j > 0; j--){
				//if the current element is not zero then
				if(board[j][i] != 0){
					//if the current element is equal to the element above it then
					if(board[j][i] == board[j-1][i]){
						//the move is valid;
						check = true;
					}
				}else{ //if the current element is zero
					//if the element of the current element isn't zero then
					if(board[j-1][i] != 0){
						//the move is valid
						check = true;
					}
				}
			}
		}
	//if the player is moving to the right
	}else if(move == "d"){
		//for each horizontal strip
		for(int i = 0; i < ROWS; i++){
			//starting from the right going to the element second from the left
			for(int j = COLS - 1; j > 0; j--){
				//if the current element is not 0 then
				if(board[i][j] != 0){
					//if the current element is the same as the element to the left of it
					if(board[i][j] == board[i][j-1]){
						//the move is valid
						check = true;
					}
				}else{ //if the current element is zero
					//if the element to the left of the current element is not zero then
					if(board[i][j-1] != 0){
						// the move is valid
						check = true;
					}
				}
			}
		}
	}else{
		//any other input move is invalid
		check = false;
	}
	//return the check variable
	return check;
}