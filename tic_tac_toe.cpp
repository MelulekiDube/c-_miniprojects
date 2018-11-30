#include <cstdlib>
#include <iostream>
#include <cstring>

using namespace std;
string VALUES = "123456789";
int num_players;
char number_base = '0';

char ai_player, opponent;


extern void play(char player);

void print_current_values()
{
	cout << "------------------------\n" << endl;
	for(int i = 0; i < 3; ++i)
	{
		cout << "       |       |" << endl;
		for(int j =0; j <3; ++j)
		{
			cout << "   " << VALUES[(3*i+j)];
			if(j != 2)
				cout << "   |";
		}
		cout << endl;
		if( i !=2 )
			cout << "_______|_______|_______" << endl;
		else
			cout << "       |       |" << endl;
	}
}

bool winning(char player)
{
	return (player == VALUES[0]&& player == VALUES[1]&& player == VALUES[2])||
		(player == VALUES[3] && player == VALUES[4] && player == VALUES[5])||
		(player == VALUES[6] && player == VALUES[7] && player == VALUES[8])||
		(player == VALUES[0] && player == VALUES[4] && player == VALUES[8])||
		(player == VALUES[2] && player == VALUES[4] && player == VALUES[6])||
		(player == VALUES[0] && player == VALUES[3] && player == VALUES[6])||
		(player == VALUES[1] && player == VALUES[4] && player == VALUES[7])||
		(player == VALUES[2] && player == VALUES[5] && player == VALUES[8]);
}

//checks if the current position is not already filled with an X or O
bool is_cell_empty(int position)
{
	return !(VALUES[position]=='X'||VALUES[position] == 'O');
}

// evaluate if there are any empty moves in the board
bool moves_left(void)
{
	for(int i=0; i < 9; ++i)
	{
		if(is_cell_empty(i))
		{
			return true;
		}
	}
	return false;
}

// evaluates the board and gives an appropriate score
// a score of greater than 0 is given if X is winning
// a score less than 0 is given if O is winning the game
// otherwise we just give a score of 0
int evaluate()
{
	if(winning(ai_player))
		return 10;
	else if(winning(opponent))
		return -10;
	return 0;
}

// Returns the optimal value a maximizer can obtain. 
// depth is current depth in game tree.  
// isMax is true if current move is if maximizer, else false 
int minimax(int depth, bool isMax) 
{
	int score = evaluate();
	//if maximizer has won the game
	if(score == 10)
		return score-depth;
	//if minizer has won the game
	if(score == -10)
		return score+depth;
	//if the game is a tie
	if(!moves_left())
		return 0;
	if(isMax)
	{
		int bestmove = -10000;	
		//evaluate for all the moves left.
		for(int i = 0; i < 9; ++i)
		{
			if(is_cell_empty(i))
			{
				//make move and see what happens
				VALUES[i] = ai_player;
				bestmove = max(bestmove, minimax(depth+1, !isMax));
				//undo the move made initially
				VALUES[i] = '0'+i+1;
			}
		}
		return bestmove;
	}
	else
	{
		int bestmove = 10000;
		for(int i = 0; i < 9; ++i)
		{
			if(is_cell_empty(i)){
				VALUES[i] = opponent; //make the move and evaluate wat will happen
				bestmove = min(bestmove, minimax(depth+1, !isMax));
				VALUES[i] = '0'+i+1; //undo the move made initially
			}
		}
		return bestmove;
	}
} 

//evaluate which is the best move to make
int find_bestmove()
{
	int bestVal = -1000;
	int bestmove = -1;
	//traverse and evaluate minmax on empty cells
	//return the cell with optimal value
	for (int i =1 ; i < 10; ++i)
	{
		if(is_cell_empty(i-1))
		{
			VALUES[i-1] = ai_player;
			int moveValue = minimax(0, false);
			VALUES[i-1] = '0'+i;
			if(moveValue > bestVal)
			{
				bestVal = moveValue;
				bestmove = i;
			}
		}
	}
	return bestmove;
}

int get_userinput()
{
	
	int position;
	cin >> position;
	if(position > 9 || position < 0)
	{
		cout << "Please the number should be between 1 and 9 inclusive: " << endl;
		cin >> position;
		cout << endl;
	}
	if(VALUES[position-1] == 'O' || VALUES[position-1]== 'X')
	{
		cout << "Invalid move. Position occupied already. Play another move: ";
		cin >> position;
		cout << endl;
	}
	return position;
}

void game_over(char winner)
{
	print_current_values();
	cout << winner << " wins the game. Game over" << endl;
}

void play_1player(void)
{
	cout << "choose X or O" << endl;
	cin >> opponent;
	ai_player = (opponent=='X') ? 'O' : 'X';
	int ai_move;
	while(moves_left())
	{
		print_current_values();
		cout << "Player 1[X]: ";
		if(opponent == 'X')
		{
			VALUES[get_userinput()-1] = 'X';
			if(winning('X'))
			{
				game_over('X');
				return;
			}
			cout << "\n" << endl;
			print_current_values();
			if(!moves_left())
				break;
			ai_move = find_bestmove();
			cout << "Player 2[O]: ";
			cout << ai_move <<endl;
			VALUES[ai_move-1] = 'O';
			if(winning('O'))
			{
				game_over('O');
				return;
			}
			cout << "\n" << endl;
		}
		else if(opponent == 'O')
		{
			ai_move = find_bestmove();
			cout << ai_move <<endl;
			VALUES[ai_move-1] = 'X';
			if(winning('X'))
			{
				game_over('X');
				return;
			}
			cout << "\n" << endl;
			print_current_values();
			cout << "Player 2[O]: ";
			VALUES[get_userinput()-1] = 'O';
			if(winning('O'))
			{
				game_over('O');
				return;
			}
			cout << "\n" << endl;
		}
	}
	cout << "Game is a tie. Game over." << endl;
}

void play_2player(void)
{
	while(moves_left())
	{
		print_current_values();
		cout << "Player 1[X]: ";
		VALUES[get_userinput()-1] = 'X';
		if(winning('X'))
		{
			game_over('X');
			return;
		}
		cout << "\n" << endl;
		print_current_values();
		if(!moves_left())
			break;
		cout << "Player 2[O]: ";
		VALUES[get_userinput()-1] = 'O';
		if(winning('O'))
		{
			game_over('O');
			return;
		}
		cout << "\n" << endl;
	}
	cout << "Game is a tie. Game over." << endl;
}
void start_game(void) 
{	
	if(num_players == 1)
		play_1player();
	else
		play_2player();
}

int main(int argc, char * args[])
{
	cout << "TIC TAC TOE GAME" <<endl;
	cout << "Player1 is [X] and player2 is [O]" <<endl;
	if(argc == 1 || !strcmp(args[1], "1"))
	{
		num_players = 1;
	}
	else if(argc == 2 && !strcmp(args[1], "2"))
	{
		num_players = 2;
	}
	else 
		cout << "invalid arguments" <<endl;
	if(num_players != 0)
		start_game();
	return 0;
}