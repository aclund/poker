#include "functions.h"

void calculate( vector<int> playing_board, int score[2] ) {

	score[0] = 0; score[1] = 0;

#ifdef DEBUG
	// Print Board
	cout << "Playing Board  : ";
	for( int i : playing_board ) {
		cout << i << " ";
	}
	cout << "\n";
#endif

	// Find Pair/Trip/Quad - Max 3 Pairs
	int pair;
	int pair_pt[3] = { 0, 0, 0 };
	for( int i=0; i<playing_board.size()-1; i++ ) {
		for( int j=i+1; j<playing_board.size(); j++ ) {
			if( playing_board[i]%13 == playing_board[j]%13 ) {
				pair = playing_board[i]%13 + 1;
				if( pair == 1 ) pair += 13;
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
#ifdef DEBUG
	cout << "Pairs/Trips/Quads: ";
	for( int k=0; k<3; k++ ) {
		cout << pair_pt[k] << "  ";
	}
	cout << "\n";
#endif

	// First sort (merge sort) into int_board
	int int_board[playing_board.size()+1];
	for( int i=0; i<playing_board.size(); i++ ) {
		int_board[i] = playing_board[i]%13;
	}
#ifdef DEBUG
	cout << "Int_board: ";
	for( int i=0; i<playing_board.size(); i++ ) {
		cout << int_board[i] << " ";
	}
	cout << "\n";
#endif
	quickSort( int_board, 0, playing_board.size()-1 );
	// Ace goes both ways
	int end = playing_board.size();
	if( int_board[0] == 0 ) {
		int_board[playing_board.size()] = 13;
		end = playing_board.size()+1;
	}
#ifdef DEBUG
	cout << "Int_board sort: ";
	for( int i=0; i<end; i++ ) {
		cout << int_board[i] << " ";
	}
	cout << "\n";
#endif

	// Straight
	int straight = 0, straight_flush = 0;
	for( int i=0; i<end-4; i++ ) {
		int last = int_board[i];
		int k = i-1;
		int repeat = 0;
		while( int_board[k] == last ) {
			repeat++;
			k--;
		}
		int count = 1;
		for( int j=i+1; j<playing_board.size()+1; j++ ) {
			if( int_board[j] == last + 1 ) {
				count++;
				last++;
			}
			else if( int_board[j] != last ) { break; }
			if( count == 5 ) {
				straight = last;
#ifdef DEBUG
				cout << "search straight " << straight << "\n";
#endif
				int suit = -1;
				for( int n_board : playing_board ) {
					if( n_board%13 == straight-4 ) {
						suit = int(n_board/13);
						if( repeat > 0 ) { repeat--; }
						else { break; }
					}
				}
				int count_flush = 0;
				for( int k=straight-4; k<=straight; k++ ) {
					for( int n_board : playing_board ) {
						if( n_board%13 == k && int(n_board/13) == suit ) {
							count_flush++;
						}
					}
				}
#ifdef DEBUG
				cout << "straight_flush " << count_flush << " suit " << suit <<"\n";
#endif
				if( count_flush == 5 ) { straight_flush = straight; }
			}
		}
	}
#ifdef DEBUG
	cout << "Straight: " << straight << "\n";
#endif

	// Find Flush
	int spades   = 0;
	int hearts   = 0;
	int clubs    = 0;
	int diamonds = 0;
	for( int i=0; i<playing_board.size(); i++ ) {
		if(      playing_board[i] < 13 ) spades++;
		else if( playing_board[i] < 26 ) hearts++;
		else if( playing_board[i] < 39 ) clubs++;
		else if( playing_board[i] < 52 ) diamonds++;
		else { cout << "ERROR: suit NOT found!\n"; }
	}
	int flush = 0;
	if( spades   >= 5 ) flush = 4;
	if( hearts   >= 5 ) flush = 3;
	if( clubs    >= 5 ) flush = 2;
	if( diamonds >= 5 ) flush = 1;
#ifdef DEBUG
	cout << "Flush: " << flush << "\n";
#endif

	/////////////////
	// Find best hand

	int found, tie_break, tie_break1;

	// Straight Flush
	if( straight_flush != 0 ) {
		score[0] = 8;
		score[1] = straight_flush;
		return;
	}

	// Quads
	for( int i=0; i<3; i++ ) {
		if( pair_pt[i] > 500 ) {
			score[0] = 7;
			int quad = pair_pt[i]%100;
			for( int j=0; j<playing_board.size()+1; j++ ) {
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
			for( int j=playing_board.size(); j>0; j-- ) {
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
			temp       = tie_break;
			tie_break  = tie_break1;
			tie_break1 = temp;
		}

		if( pair_pt[i] ) {
			found++;
			if( !tie_break ) { tie_break = pair_pt[i]; }
			else { tie_break1 = max( tie_break1, pair_pt[i] ); }
		}
		// tie_break is top pair
		if( tie_break < tie_break1 ) {
			temp       = tie_break;
			tie_break  = tie_break1;
			tie_break1 = temp;
		}
	}
	if( found > 1 ) {
		score[0] = 2;
		score[1] = 1000*tie_break + 100*tie_break1;
		for( int j=playing_board.size(); j>0; j-- ) {
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
		for( int j=playing_board.size(); j>0; j-- ) {
			if( int_board[j] != pair_pt[0] & tie_break < 3 ) {
				score[1] += int_board[j];
				tie_break++;
			}
		}
	}
	if( found ) return;

	// High Card
	for( int j=playing_board.size(); j>0; j-- ) {
		score[1] += int_board[j];
	}

	return;
}
