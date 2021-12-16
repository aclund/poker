#include <iostream>
#include <string>

using namespace std;

#include "functions.h"
#include "deck.h"

int main(int argc, char *argv[]) {

	// Initialize deck of cards
	int index = 0;
	for( int i = 0; i < sizeof(suits)/sizeof(suits[0]); i++ ) {
		for( int j = 0; j < sizeof(values)/sizeof(values[0]); j++ ) {
			cards[index] = string(1,values[j]) + suits[i];
			index++;
		}
	}

/*
	// Get hand and board
	string in_hand, hand[2], in_board, board[5];
	cout << " Enter HAND : ";
	getline(cin,in_hand);
	cout << " Enter BOARD : ";
	getline(cin,in_board);
	int ierr = convert_in( in_hand, hand, in_board, board, cards );
	if( ierr != 0 ) return 0;
*/
	string  hand[2] = { "5H", "3H" };
	string board[5] = { "TD", "TS", "TC" };

	// Find current showdown
	int score[2];
	calculate( board, hand, score );
	cout << "Current Score: " << score[0] << "  " << score[1] << endl;

	// Calculate drawing odds
	odds( cards, board, hand );
	
	return 0;
}
