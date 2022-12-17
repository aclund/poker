#include "functions.h"

string uppercase(string s) {
	transform(s.begin(), s.end(), s.begin(), ::toupper);
	return s;
}

int convert_in( string read_cards, vector<int> &playing_board ) {

	int ierr = 0;

	// UpperCase Inputs
	read_cards = uppercase(read_cards);

	// Only spaces allowed rn
	char delim = ' ';

	char values[13] = { 'A', '2', '3', '4', '5', '6', '7',
			    '8', '9', 'T', 'J', 'Q', 'K' };
	char suits[4]   = { 'S', 'H', 'C', 'D' };
	string cards[52];

	// Initialize deck of cards
	int index = 0;
	for( int i = 0; i < sizeof(suits)/sizeof(suits[0]); i++ ) {
		for( int j = 0; j < sizeof(values)/sizeof(values[0]); j++ ) {
			cards[index] = string(1,values[j]) + suits[i];
			index++;
		}
	}

	// Assign hand
	int i = 0, start = 0, end = 0;
	string tmp;
	while( start != string::npos ) {
		while( read_cards[start] == delim ) {
			start++;
		}
		end = read_cards.find(delim, start);
		if( end == string::npos ) break;

		tmp = read_cards.substr(start, end - start);

		bool found = false;
		for( int k = 0; k < 52; k++ ) {
			if( tmp == cards[k] ) {
				found = true;
				playing_board.push_back(k);
			}
		}
		if( !found ) ierr = 1;

		start = end;
		i++;
	}
	if( ierr != 0 ) { cout << "ERROR: Read card NOT found!\n"; }

	// Check size of playing board
	if( playing_board.size() != 2 && playing_board.size() != 5 &&
	    playing_board.size() != 6 && playing_board.size() != 7 ) {
		cout << "ERROR: playing board wrong size = " <<
			playing_board.size() << "\n";
		ierr = 1;
	}

	// Check repeat cards
	for( int j=0; j<52; j++ ) {
		int count = 0;
		for( int i : playing_board ) {
			if( i == j ) count++;
		}
		if( count > 1 ) {
			cout << "ERROR: Repeat card found! " << j << "\n";
			ierr = 1;
		}
	}

//#ifdef DEBUG
	cout << "Playing Board  : ";
	for( int i : playing_board ) {
		cout << values[i%13] << suits[int(i/13)] << " ";
	}
	cout << "\n\n";
//#endif

	return ierr;
}
