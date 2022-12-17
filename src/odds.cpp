#include "functions.h"

void odds( vector<int> playing_board ) {

	// Exclude current cards
	vector<int> ex_deck;
	int count = 0;
	bool found;
	for( int i = 0; i < 52; i++ ) {
		found = false;
		for( int j : playing_board ) {
			if( i == j ) found = true;
		}
		if( !found ) {
			ex_deck.push_back(i);
			count++;
		}
	}

	//  Tracker stores scores from each combinations.
	//  First 9 indices increment on outcomes
	//  Last index counts total
	int tracker[10] = {0,0,0,0,0,0,0,0,0};

	//  Calculate every combination of ex_deck and track outcomes
	int n_add = 7-playing_board.size();
	int data[n_add];
	combinations( ex_deck, 0, data, 0, n_add, playing_board, tracker );

	//  Total boards calculated
	int total = 0;
	for( int i=0; i<9; i++ ) {
		total += tracker[i];
	}

#ifdef DEBUG
	cout << "Boards Calculated " << total << "\n";
#endif
	cout << "0 Pair         : " << float(tracker[0])/float(total)*100 << "\n";
	cout << "1 Pair         : " << float(tracker[1])/float(total)*100 << "\n";
	cout << "2 Pair         : " << float(tracker[2])/float(total)*100 << "\n";
	cout << "Trips          : " << float(tracker[3])/float(total)*100 << "\n";
	cout << "Straight       : " << float(tracker[4])/float(total)*100 << "\n";
	cout << "Flush          : " << float(tracker[5])/float(total)*100 << "\n";
	cout << "Full House     : " << float(tracker[6])/float(total)*100 << "\n";
	cout << "Quads          : " << float(tracker[7])/float(total)*100 << "\n";
	cout << "Straight Flush : " << float(tracker[8])/float(total)*100 << "\n\n";

	return;
}
