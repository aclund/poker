#include <iostream>
#include <string>

using namespace std;

#include "functions.h"
#include "deck.h"

inline void reset( string a[7], string b[7], int s ) {
	for( int i = 0; i < s; i++ ) {
		a[i] = b[i];
	}
	for( int i = s; i < 7; i++ ) {
		b[i] = "";
	}
	return;
}

void odds( string cards[52], string board[5], string hand[2] ) {

	string playing_board[7] = { hand[0], hand[1] };
	int add = 0;
	while( !board[add].empty() ) {
		playing_board[add+2] += board[add];
		add++;
	}

	// Determine size of playing board - Could be pre-flop/flop/turn/river
	int board_size = 0;
	while( !playing_board[board_size].empty() ) {
		board_size++;
		if( board_size == 7 ) return;
	}

	// Exclude current cards
	string ex_deck[52-board_size];
	int count = 0;
	bool found;
	for( int i = 0; i < 52; i++ ) {
		found = false;
		for( int j = 0; j < board_size; j++ ) {
			if( cards[i] == playing_board[j] ) found = true;
		}
		if( !found ) {
			ex_deck[count] = cards[i];
			count ++;
		}
	}

	// Critical loop
	string tmp_board[7];
	reset( tmp_board, playing_board, board_size );

	for( int i = board_size - 7; i < 7; i++ ) {
		for( int j = 0; j < 52-board_size; j++ ) {
			
		}
	}

	return;
}
