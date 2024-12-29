/*---------------------------------------------------*/
/* Name: Aman Sunesh, NetID: as18181                 */
/* Date: December 3, 2023                            */
/* Program: IteratedPrisonersDilemma.cpp             */
/* Description: This program implements the Iterated */
/* Prisoner's Dilemma (IPD) game, a classic scenario */
/* in game theory. Players, represented by  distinct */
/* strategies,  repeatedly  choose  to  cooperate or */
/* defect  based on  their opponent'  previous move. */
/* The strategies include Random (r), Cooperate (c), */
/* Evil (e), and Tit for Tat (t).The game progresses */
/* through multiple  rounds, calculating  scores for */
/* each player based on the interactions. The winner */
/* is determined by the highest cumulative score.The */
/* program provides a menu-driven interface for user */
/* interactions, allowing them to  add/drop players, */
/* set the number  of rounds, choose strategies, and */
/* initiate  the   game  simulation.  Proper  memory */
/* management  is  ensured  for  a  clean execution. */
/*---------------------------------------------------*/


#include <iostream>
#include <cstdlib>
#include <string>
#define Max_Players 2

using namespace std;

//Function prototype for input validation.
bool isInvalidInput();

//Class for generating unique IDs
class generateID
{
private:
	static int ID;

public:

	//Get the next available unique ID.
	static int getID()
	{
		ID++;
		return ID;
	}

	//Reset the static ID variable in the generateID class back to zero.
	static void resetID()
	{
		ID = 0;
	}
};

//Initializing static member ID of generateID class
int generateID::ID = 0;

//Class defining different strategies for the players
class Strategy
{
private:
	char strategyCode;


public:
	//Default Constructor
	Strategy()
	{
		strategyCode = 'r'; //Default strategy is Random
	}


	//Setter for strategyCode
	void setStrategyCode(char code)
	{
		strategyCode = code;
	}

	//Getter for strategyCode
	char getStrategyCode()
	{
		return strategyCode;
	}

	//Function to determine the move based on the strategy
	char cooperateOrDefect(char opponentLastMove)
	{
		switch (strategyCode)
		{

			case 'r':  // Random
			{
				if (rand() % 2 == 0) {
					return 'c';
				}
				else {
					return 'd';
				}
			}

			case 'c':  // Cooperate
				return 'c';

			case 'e':  // Evil
				return 'd';

			case 't':  // Tit for Tat
				return opponentLastMove;

			default:
			{
				cout << "Error: Invalid Strategy Code" << endl;
				return '\0';
			}
		}
	}

};


//Class representing player in the game.
class Player
{
private:
	int ID;
	string name;
	int score;
	int numOfMoves;
	int totalMoves;
	char* prevMoves;
	Strategy s;
	static int numOfPlayers;


public:
	//Default Constructor
	Player()
	{
		ID = generateID::getID();
		name = "";
		score = 0;
		numOfMoves = 0;
		totalMoves = 0;
		prevMoves = nullptr;
		numOfPlayers++;
	}


	//Copy Constructor
	Player(const Player& other)
	{
		//Copy primitive data members
		ID = other.ID;
		name = other.name;
		score = other.score;
		numOfMoves = other.numOfMoves;
		totalMoves = other.totalMoves;

		if (other.prevMoves != nullptr)
		{
			prevMoves = other.prevMoves;
		}

		else
		{
			prevMoves = nullptr;
		}

		//Copy strategy (assuming that Strategy has an appropriate copy constructor)
		s = other.s;
	}

	//Accessors
	int getID()
	{
		return ID;
	}

	string getName()
	{
		return name;
	}

	int getScore()
	{
		return score;
	}

	char getStrategy()
	{
		return s.getStrategyCode();
	}

	int getNumOfMoves() {
		return numOfMoves;
	}

	char getLastMove(int x)
	{
		if (prevMoves != nullptr && x >= 0 && x < numOfMoves) 
		{
			return prevMoves[numOfMoves - (numOfPlayers - 1) + x];
		}
		else
		{
			cout << "Error! Cannot access last move";
		}
	}


	//Modifiers
	void setName(string playerName)
	{
		name = playerName;
	}

	void setNumberOfMoves(int initTotalMoves)
	{
		//Calculating total moves considering we do not know that the number of players is 2
		totalMoves = initTotalMoves * (numOfPlayers - 1); 

		prevMoves = new char[totalMoves];

	}


	void updateStrategy(char code)
	{
		s.setStrategyCode(code);
	}

	char makeMove(char opponentMove)
	{
		char move = s.cooperateOrDefect(opponentMove);

		printMoves(move);
		
		if (numOfMoves <= totalMoves)
		{
			setLastMove(move);
		}
		else
		{
			cout << "Error: Too many moves made. Move index out of bounds." << endl;
		}
		return move;
	}


	void setLastMove(char newMove)
	{
		prevMoves[numOfMoves] = newMove;
		numOfMoves++;
	}


	//Functions
	void printMoves(char move)
	{
		if (move == 'c')
		{
			cout << "Player " << ID << " Move: Cooperate" << endl;
			cout << endl;
		}

		else
		{
			cout << "Player " << ID << " Move: Defect" << endl;
			cout << endl;
		}
	}

	void increaseScore(int newScore)
	{
		score += newScore;
	}

	void resetData()
	{
		score = 0;
		numOfMoves = 0;
	}

	char* resetMoves()
	{
		return prevMoves;
	}

	static void resetNumOfPlayers()
	{
		numOfPlayers = 0;
	}

	//Destructor
	~Player()
	{
		delete[] prevMoves; // Deallocate memory for prevMoves array
	}
};

//Initializing static member numOfPlayers of Player class
int Player::numOfPlayers = 0;


//Class representing the game and its operations.
class Game
{
private:
	Player* players; //Array to store player objects
	int numOfPlayers;
	int numOfRounds;
	char strategy;

public:

	//Default Constructor
	Game()
	{
		players = nullptr;
		numOfPlayers = 0;
		numOfRounds = 0;
	}

	//Set the number of rounds and allocate memory for each player's moves
	void setNumberOfRounds(int rounds)
	{
		numOfRounds = rounds;

		
		//Allocate memory for each player's moves
		for (int i = 0; i < numOfPlayers; i++)
		{	
			// Reset player-specific data
			players[i].resetData();

			// Set the number of moves for each player based on the specified rounds
			players[i].setNumberOfMoves(rounds);
		}
	}


	//Display the result of the game
	void displayResult()
	{
		int winnerID = -1, highestScore = -1;
		int* tiedPlayers = new int[numOfPlayers];  //Dynamic array to store IDs of tied players
		int numTiedPlayers = 0;
		string winner;

		cout << endl;
		cout << "*********************************" << endl;
		cout << endl;

		for (int i = 0; i < numOfPlayers; i++)
		{
			//Display player information
			cout << "Player ID: " << players[i].getID() << endl;
			cout << "Name: " << players[i].getName() << endl;
			cout << "Score: " << players[i].getScore() << endl;
			cout << endl;
			
			//Check for the winner
			if (players[i].getScore() > highestScore)
			{
				//Reset tied player count if a new highest score is found
				numTiedPlayers = 0;  
				tiedPlayers[numTiedPlayers++] = players[i].getID();
				winnerID = players[i].getID();
				winner = players[i].getName();
				highestScore = players[i].getScore();
			}

			else if (players[i].getScore() == highestScore)
			{
				if (numTiedPlayers == 0)  //Allocate memory only if it's the first tie
					tiedPlayers = new int[numOfPlayers];

				//Store the ID of a tied player
				tiedPlayers[numTiedPlayers++] = players[i].getID();
			}

		}

		cout << "*********************************" << endl;
		cout << endl;

		//Display Result
		cout << "------------------------------------" << endl;
		cout << "|              RESULT              |" << endl;
		cout << "------------------------------------" << endl;

		//Check if there is a single winner or a tie
		if (numTiedPlayers == 1)
		{
			//Display information for a single winner
			cout << "|WinnerID: " << winnerID << "                       |" << endl;
			cout << "|Name: " << winner << "                           |" << endl;
			cout << "|Score: " << highestScore << "                         |" << endl;
		}

		else
		{
			//Display information for a tie
			cout << "|It's a tie                        |" << endl;
			cout << "|                                  |" << endl;
			cout << "|Tied Players:                     |" << endl;

			for (int i = 0; i < numTiedPlayers; i++)
			{

				cout << "|Player " << tiedPlayers[i] << "                          |" << endl;
			}

			cout << "|                                  |" << endl;

			cout << "|Score of each player: " << highestScore << "          |" << endl;
		}

		cout << "------------------------------------" << endl;

		cout << endl;

		//Deallocate memory for tiedPlayers array
		delete[] tiedPlayers;
	}

	//Add players to the game
	void addPlayers(int numPlayers)
	{
		//Release memory if players array is not null
		if (players != nullptr)
		{
			delete[] players;
			generateID::resetID();
			Player::resetNumOfPlayers();

		}

		players = new Player[numPlayers];
		numOfPlayers = numPlayers;

		//Clear any remaining newline characters in the input buffer.
		cin.ignore();

		for (int i = 0; i < numOfPlayers; i++)
		{
			string playerName;

			cout << "Enter the name for Player " << i + 1 << ": ";

			//Use getline to read the entire line, allowing names with spaces.
			getline(cin, playerName);
			

			while (isInvalidInput())
			{
				cout << "Please enter a string value for the name of the player: ";

				//Use getline to read the entire line, allowing names with spaces.
				getline(cin, playerName);
			}

			players[i].setName(playerName);

			cout << "Players added successfully!" << endl;
			cout << endl;
		}
	}


	void addPlayersToExistingGame(int numOfPlayersToAdd)
	{
		//Ensure that game already exists
		if (players != nullptr)
		{
			//Check if the total number of players after adding new players is within the allowable limit
			if ((numOfPlayers + numOfPlayersToAdd) <= Max_Players)
			{
				//Create a temporary array to hold the updated players
				Player* updatePlayers = new Player[numOfPlayers + numOfPlayersToAdd];

				//Copy existing players data to the updated array and reset their individual data
				for (int i = 0; i < numOfPlayers; i++)
				{
					updatePlayers[i] = players[i];
					updatePlayers[i].resetData();

				}

				//Deallocate the memory for players array
				delete[] players;

				//Assign the updated array to Players
				players = updatePlayers;

				//Update the total number of players
				numOfPlayers += numOfPlayersToAdd;
			}

			else
			{
				cout << "Error! Max Limit Reached! Could not add new players" << endl;
			}
 
		}

		//Ignore any remaining characters in the input buffer
		cin.ignore();

		for (int i = (numOfPlayers - numOfPlayersToAdd); i < numOfPlayers ; i++)
		{
			string playerName;

			cout << "Enter the name of Player: ";

			//Use getline to read the entire line, allowing names with spaces.
			getline(cin, playerName);


			while (isInvalidInput())
			{
				cout << "Please enter a string value for the name of the player: ";

				//Use getline to read the entire line, allowing names with spaces.
				getline(cin, playerName);
			}

			players[i].setName(playerName);

			cout << "Players added successfully!" << endl;
			cout << endl;
		}


	}

	//Drop a player from the game
	void dropPlayer(int playerID)
	{
		bool flag = false;

		for (int i = 0; i < numOfPlayers; ++i) {
			if (players[i].getID() == playerID) {
				for (int j = i; j < numOfPlayers - 1; j++)
				{
					players[j] = players[j + 1];
				}

				//Initialize last player object to zero in order to remove duplicate player objects
				players[numOfPlayers - 1] = Player();
				numOfPlayers--;

				flag = true;
				cout << "Player " << playerID << " successfully deleted" << endl;

				//Dynamic memory allocated to the dropped player object is deallocated at the end of main

				break;
			}
		}

		if (flag == false)
		{
			cout << "Player " << playerID << " not found" << endl;
		}
	}


	//Get information about the players
	Player* getPlayerInfo() {
		return players;
	}


	//Start the game
	void play()
	{

		for (int i = 0; i < numOfRounds; i++)
		{
			for (int j = 0; j < numOfPlayers; j++)
			{
				int x = 0;

				for (int k = 0; k < j; k++)
				{
					cout << "----------------------------------" << endl;
					cout << "              Round " << i + 1 << "             " << endl;
					cout << "----------------------------------" << endl;
					cout << endl;

					//Stores Player Moves
					char playerOne = 'c';
					char playerTwo = 'c';

					if (i == 0)
					{
						//For the first round, the opponent's last move is set as 'c'.
						//This does not impact the outcome since we only need the last move if the user chooses "tit for tat."
						//If the user chooses "tit for tat," the program prompts the user for their first move.
						//Otherwise, the first move is determined based on the player's selected strategy.
						char defaultChar = 'c';


						if (players[j].getStrategy() == 't')
						{
							cout << "Player " << j + 1 << ", please enter your first move: Enter 'c' for cooperate or 'd' for defect: ";
							cin >> defaultChar;

							while (isInvalidInput() || (defaultChar != 'c' && defaultChar != 'd'))
							{
								cout << "Please enter either 'c' or 'd' for your first move: ";
								cin >> defaultChar;
							}


							playerOne = players[j].makeMove(defaultChar);
						}

						else
						{
							playerOne = players[j].makeMove(defaultChar); //If selected strategy is not tit for tat
						}


						if (players[k].getStrategy() == 't')
						{
							cout << "Player " << k + 1 << ", please enter your first move: Enter 'c' for cooperate or 'd' for defect: ";
							cin >> defaultChar;

							while (isInvalidInput() || (defaultChar != 'c' && defaultChar != 'd'))
							{
								cout << "Please enter either 'c' or 'd' for your first move: ";
								cin >> defaultChar;
							}

							playerTwo = players[k].makeMove(defaultChar);
						}

						else
						{
							playerTwo = players[k].makeMove(defaultChar); //If selected strategy is not tit for tat
						}

					}


					else
					{
						int last_Move2 = players[k].getLastMove(x);
						int last_Move1 = players[j].getLastMove(x);

						playerOne = players[j].makeMove(last_Move2);
						playerTwo = players[k].makeMove(last_Move1);
					}
				
					x = x + 1;

					//Set Score based on moves
					if (playerOne == 'c' && playerTwo == 'c')
					{
						players[j].increaseScore(3);
						players[k].increaseScore(3);
					}
					else if (playerOne == 'c' && playerTwo == 'd')
					{
						players[j].increaseScore(0);
						players[k].increaseScore(5);
					}
					else if (playerOne == 'd' && playerTwo == 'c')
					{
						players[j].increaseScore(5);
						players[k].increaseScore(0);
					}
					else // move1 == 'd' && move2 == 'd'
					{
						players[j].increaseScore(1);
						players[k].increaseScore(1);
					}

				}
			}


		}
		displayResult(); // Display the final result of the game
	}

	//Destructor
	~Game() 
	{
		delete[] players; //Deallocate memory for players array
	}

};


//Main function
int main()
{
	//Seed the random number generator for generating random moves in the game
	srand(time(NULL));

	//Declare and Initialize Variables
	int choice1 = 0, choice2 = 0;
	char choice3;
	int numOfPlayers = 0;
	int numOfRounds = 0;
	int addPlayers = 0; 
	string name = "";

	//Flags to track which steps have been completed
	bool playersCreated = false;
	bool setNumberOfRounds = false;
	bool setStrategy = false;

	//Create a Game Object
	Game G;


	//User Interface
	do
	{	
		//Display menu options
		cout << endl;
		cout << "---------------------------------------------------" << endl;
		cout << "            ITERATED PRISONER'S DILEMMA            " << endl;
		cout << "---------------------------------------------------" << endl;
		cout << "1. Add/Drop Players" << endl;
		cout << "2. Set Number of Rounds" << endl;
		cout << "3. Choose your Game Strategy" << endl;
		cout << "4. Start the Game" << endl;
		cout << "5. Exit" << endl;
		cout << "Enter your choice (1-5): ";
		cin >> choice1;
		cout << endl;		


		switch (choice1)
		{

			case 1:
			{
				//Option 1: Add/Drop Players

				cout << endl;
				cout << "1. Add New Players to the Game (Begin/Reset Game)" << endl;
				cout << "2. Add Players to the Existing Game" << endl;
				cout << "3. Drop Players" << endl;
				cout << "Enter your choice (1-3): ";
				cin >> choice2;

				while (isInvalidInput() || (choice2 != 1 && choice2 != 2 && choice2 != 3))
				{
					cout << "Invalid Choice! Please enter either 1,2 or 3: ";
					cin >> choice2;
				}


				if (choice2 == 1)
				{
					//Add Players
					cout << "Enter the number of players (exactly 2 players required): " << endl;
					cin >> numOfPlayers; 

					while (isInvalidInput() || numOfPlayers != Max_Players)
					{
						cout << "Error! Please enter exactly 2 players: ";
						cin >> numOfPlayers;
					}

					//Number of players can be increased by modifying the condition of the while loop

					//Add new players to the game
					G.addPlayers(numOfPlayers);
					playersCreated = true;
				}

				
				else if (choice2 == 2)
				{
					if (!playersCreated)
					{
						cout << "Error: Create the players first (Option 1)!" << endl;
						break;
					}

					
					if (numOfPlayers == Max_Players)
					{
						cout << "Error! Max Limit Reached! Could not add new players";
						break;
					}

					bool maxReached = false;


					cout << "Enter the number of players to add to the game" << endl;
					cin >> addPlayers;

					while (isInvalidInput() || addPlayers < 1 || (numOfPlayers + addPlayers) > 2)
					{
						if (addPlayers < 1)
						{
							cout << "Please enter at least one player to add: " << endl;
							cin >> addPlayers;
						}

						else if ((numOfPlayers + addPlayers) > 2)
						{
							//For the context of this assignment, the number of players has to be 2
							cout << "Please enter a maximum of 2 players to start the game(enter exactly 2 players): ";
							cin >> addPlayers;
							maxReached = true;
							break;

						}
					}

					if (!maxReached) //Executes if and only if number of players is within the allowed limit
					{
						//Add additional players to the game
						G.addPlayersToExistingGame(addPlayers);

						numOfPlayers += addPlayers;
					}

					else
					{
						continue;
					}

					break;
				}


				else if (choice2 == 3)
				{
					//Drop Players
					if (!playersCreated)
					{
						cout << "Error: Create the players first (Option 1)!" << endl;
						break;
					}

					int pID;

					cout << "Enter the ID of the player you want to drop: " << endl;
					cin >> pID;

					while (isInvalidInput() || pID < 0)
					{
						cout << "Please enter a non-negative integer value for ID: ";
						cin >> pID;
					}

					//Drop the specified player
					G.dropPlayer(pID);
					numOfPlayers--;
				}

				break;
			}

			case 2:
			{
				//Option 2: Set Number of Rounds

				//Ensure players are created before setting the number of rounds
				if (!playersCreated)
				{
					cout << "Error: Create the players first (Option 1)!" << endl;
					break;
				}

				bool flag = true;


				if (!flag)
				{
					for (int i = 0; i < numOfPlayers; i++)
					{
						//Deallocate memory for the player's previous moves
						delete[] G.getPlayerInfo()[i].resetMoves();

					}

					flag = true;

				}

				cout << "Enter the number of rounds: ";
				cin >> numOfRounds;

				while (isInvalidInput() || numOfRounds <= 0)
				{
					cout << "Please enter a positive value for number of rounds: ";
					cin >> numOfRounds;
				}

				//Set the number of rounds in the game
				G.setNumberOfRounds(numOfRounds);

				setNumberOfRounds = true;
				flag = false;
				break;
			}


			case 3:
			{
				//Option 3: Choose your game strategy

				//Ensure players and the number of rounds are set before choosing strategies.
				if (!playersCreated || !setNumberOfRounds)
				{
					cout << "Error: Create the players, and set the number of rounds first (Options 1, and 2)!" << endl;
					break;
				}

				//Display available game strategies
				cout << endl;
				cout << "==================================" << endl;
				cout << "          GAME STRATEGIES         " << endl;
				cout << "==================================" << endl;
				cout << "Enter r for Random" << endl;
				cout << "Enter c for Cooperate" << endl;
				cout << "Enter e for Evil" << endl;
				cout << "Enter t for Tit for Tat" << endl;
				cout << endl;


				//Prompt each player to choose a strategy
				for (int i = 0; i < numOfPlayers; i++)
				{
					do
					{
						cout << "Player " << i + 1 << " , Please choose your strategy: ";
						cin >> choice3;

						if (choice3 != 'r' && choice3 != 'c' && choice3 != 'e' && choice3 != 't')
						{
							cout << "Please either 'r', 'c', 'e', or 't' for your strategy : " << endl;
						}

						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');

					} while (isInvalidInput() || (choice3 != 'r' && choice3 != 'c' && choice3 != 'e' && choice3 != 't'));

					//Set the chosen strategy for the player
					G.getPlayerInfo()[i].updateStrategy(choice3);

				}

				setStrategy = true;
				break;
			}

			case 4:
			{
				//Option 4: Start the game

				//Ensure players, the number of rounds, and strategies are set before starting the game.
				if (!playersCreated || !setNumberOfRounds || !setStrategy)
				{
					cout << "Error: Create the players, set the number of rounds, and choose strategy first (Options 1, 2, and 3)!" << endl;
					break;
				}

				//Start the game
				G.play();

				break;
			}


			case 5:
			{
				//Option 5: Exit

				cout << "Thank you for playing!" << endl;

				cout << endl;

				cout << "End of the program!" << endl;
				break;
			}


			default:
			{
				//Handle invalid choices
				cout << "Invalid Choice! Please enter either 1, 2, 3, 4, or 5. ";

				//Clears the input buffer
				cin.clear();

				//To ensure that error message is printed only once
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
		}

	} while (choice1 != 5);

	return 0;
}


//Function to check if the input is valid
bool isInvalidInput()
{
	bool invalidInput = false;

	//Check for invalid input
	if (cin.fail())
	{
		invalidInput = true;
		cout << "Error! Invalid Input! Please enter a valid numeric input!" << endl;
		cout << endl;

		//Clears the input buffer
		cin.clear();

		//To ensure that error message is printed only once
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

	}

	else
	{
		invalidInput = false;
	}

	return invalidInput;
}
