#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

#include "functions.h"
#include "deck.h"
#include "conv_value.h"

void calculate( string board[5], string hand[2], int score[2] ) {

	// Create contraction of hand + board
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
		if( board_size == 7 ) break;
	}


/*
	// Test Print
	cout << "Board: ";
	for( int i=0; i<board_size; i++ ) {
		cout << playing_board[i] << " ";
	}
	cout << endl;
*/

	// Find Pair/Trip/Quad - Max 3 Pairs
	int pair;
	int pair_pt[3] = { 0, 0, 0 };
	for( int i=0; i<board_size-1; i++ ) {
		for( int j=i+1; j<board_size; j++ ) {
			if( playing_board[i][0] == playing_board[j][0] ) {
				pair = conv_value( playing_board[i][0] );
				for( int k=0; k<3; k++ ) {
					if( !pair_pt[k] ) {
						pair_pt[k] = pair;
						break;
					}
					else if( pair == pair_pt[k]%100 ) {
						pair_pt[k] += 100;
						break;
					}
				}
			}
		}
	}

/*
	cout << "Pairs/Trips/Quads: ";
	for( int k=0; k<3; k++ ) {
		cout << pair_pt[k] << "  ";
	}
	cout << endl;
*/

	// First sort (merge sort) into int_board
	int int_board[board_size+1];
	for( int i=0; i<board_size; i++ ) {
		int_board[i+1] = conv_value( playing_board[i][0] );
	}
	mergeSort( int_board, 1, board_size );
	// Ace goes both ways
	int start = 0;
	if( int_board[board_size] == 14 ) {
		int_board[0] = 1;
		start = 0;
	}
	else {
		int_board[0] = 0;
		start = 1;
	}
/*
	for( int i=0; i<board_size+1; i++ ) {
		cout << int_board[i] << " ";
	}
	cout << endl;
*/
	// Straight
	int straight = 0, count = 1;
	int search_len = board_size - 4 + (-1*start + 1);
	int last;
	for( int i=start; i<search_len; i++ ) {
		last = int_board[i];
		count = 1;
		for( int j=i+1; j<board_size+1; j++ ) {
			if( int_board[j] > last + 1 ) {
				break;
			}
			else if( int_board[j] == last + 1 ) {
				count++;
				last++;
			}
			if( count == 5 ) straight = int_board[j];
		}
	}

	//cout << "Straight: " << straight << endl;

	// Find Flush
	int spades   = 0;
	int hearts   = 0;
	int clubs    = 0;
	int diamonds = 0;
	for( int i=0; i<board_size; i++ ) {
		if( playing_board[i][1] == 'S' ) spades++;
		if( playing_board[i][1] == 'H' ) hearts++;
		if( playing_board[i][1] == 'C' ) clubs++;
		if( playing_board[i][1] == 'D' ) diamonds++;
	}
	int flush = 0;
	if( spades   >= 5 ) flush = 4;
	if( hearts   >= 5 ) flush = 3;
	if( clubs    >= 5 ) flush = 2;
	if( diamonds >= 5 ) flush = 1;

	//cout << "Flush: " << flush << endl;

	/////////////////
	// Find best hand

	int found, tie_break, tie_break1;

	// Stright Flush
/* Does not account for only five card
	if( straight != 0 & flush != 0 ) {
		score[0] = 8;
		score[1] = straight;
		return;
	}
*/
	// Quads
	for( int i=0; i<3; i++ ) {
		if( pair_pt[i] > 500 ) {
			score[0] = 7;
			int quad = pair_pt[i]%100;
			for( int j=0; j<board_size+1; j++ ) {
				if( int_board[j] != quad ) {
					score[1] = max( score[1], int_board[j] );
				}
			}
			return;
		}
	}
	// Full House
	found = 0, tie_break = 0;
	for( int i=0; i<3; i++ ) {
		if( pair_pt[i] > 200 ) {
			found += 3;
			tie_break = pair_pt[i]%100;
		}
		else if( pair_pt[i] ) {
			found += 1;
			tie_break1 = max( tie_break1, pair_pt[i] );
		}

		if( found >= 4 ) {
			score[0] = 6;
			score[1] = 100*tie_break + tie_break1;
			return;
		}
	}
	// Flush
	if( flush ) {
		score[0] = 5;
		score[1] = flush;
		return;
	}
	// Straight
	if( straight ) {
		score[0] = 4;
		score[1] = straight;
		return;
	}
	// Trips
	found = 0, tie_break = 0, tie_break1 = 0;
	for( int i=0; i<3; i++ ) {
		if( pair_pt[i] > 200 ) {
			found = 1;
			score[0] = 3;
			tie_break = pair_pt[i]%100;
			score[1] = max( score[1], 100*tie_break );
			for( int j=board_size; j>0; j-- ) {
				if( int_board[j] != tie_break & tie_break1 < 2 ) {
					score[1] += int_board[j];
					tie_break1++;
				}
			}
		}
	}
	if( found ) return;

	// Two Pair
	found = 0, tie_break = 0, tie_break1 = 0;
	int temp;
	for( int i=0; i<3; i++ ) {
		// tie_break is top pair
		if( tie_break < tie_break1 ) {
			//cout << tie_break << " " << tie_break1 << endl;
			temp = tie_break;
			tie_break = tie_break1;
			tie_break1 = temp;
			//cout << tie_break << " " << tie_break1 << endl;
		}

		if( pair_pt[i] ) {
			found++;
			if(      !tie_break  ) { tie_break  = pair_pt[i]; }
			else { tie_break1 = max( tie_break1, pair_pt[i] ); }
		}
		// tie_break is top pair
		if( tie_break < tie_break1 ) {
			//cout << tie_break << " " << tie_break1 << endl;
			temp = tie_break;
			tie_break = tie_break1;
			tie_break1 = temp;
			//cout << tie_break << " " << tie_break1 << endl;
		}
	}
	if( found > 1 ) {
		score[0] = 2;
		score[1] = 1000*tie_break + 100*tie_break1;
		for( int j=board_size; j>0; j-- ) {
			if( int_board[j] != tie_break & int_board[j] != tie_break1 ) {
				score[1] += int_board[j];
				break;
			}
		}
		return;
	}

	// Pair
	found = 0, tie_break = 0, tie_break1 = 0;
	if( pair_pt[0] ) {
		found++;
		score[0] = 1;
		for( int j=board_size; j>0; j-- ) {
			if( int_board[j] != pair_pt[0] & tie_break < 3 ) {
				score[1] += int_board[j];
				tie_break++;
			}
		}
	}
	if( found ) return;

	// High Card
	for( int j=board_size; j>board_size-5; j-- ) {
		score[1] += int_board[j];
	}

	return;
}
