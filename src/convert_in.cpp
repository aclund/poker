#include <iostream>
#include <string>

using namespace std;

string uppercase(string s) {
	transform(s.begin(), s.end(), s.begin(), ::toupper);
	return s;
}

int convert_in( string in_hand, string hand[2], string in_board, string board[5], string cards[52] ) {

	// UpperCase Inputs
	in_hand  = uppercase(in_hand);
	in_board = uppercase(in_board);

	int ierr1 = 0, ierr2 = 0;

	// Only spaces allowed rn
	char delim = ' ';

	// Assign hand
	int i = 0, start = 0, end = 0;
	while( end != string::npos & i != 2 ) {
		while( in_hand[start] == delim ) {
			start++;
		}
		end   = in_hand.find(' ', start);

		hand[i] = in_hand.substr(start, end - start);

		start = end;
		i++;
	}
	for( int j = 0; j < 2; j++ ) {
		bool found = false;
		for( int k = 0; k < 52; k++ ) {
			if( hand[j] == cards[k] ) found = true;
		}
		if( !found ) ierr1 = 1;
	}
	if( ierr1 != 0 ) { cout << "Hand card NOT found!\n"; }


	// Assign board
	i = 0, start = 0, end = 0;
	while( end != string::npos & i != 5 ) {
		while( in_board[start] == delim ) {
			start++;
		}
		end   = in_board.find(' ', start);

		board[i] = in_board.substr(start, end - start);

		start = end;
		i++;
	}
	if( end == string::npos ) i--;
	for( int j = 0; j < i; j++ ) {
		bool found = false;
		for( int k = 0; k < 52; k++ ) {
			if( board[j] == cards[k] ) found = true;
		}
		if( !found ) ierr2 = 1;
	}
	if( ierr2 != 0 ) { cout << "Board card NOT found!\n"; }


	return max(ierr1,ierr2);
}
