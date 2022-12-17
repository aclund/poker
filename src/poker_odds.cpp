#include "functions.h"

int main(int argc, char *argv[]) {

	int ierr;
	string in_hand, in_board;
	bool off = false;

	// Command line arguments to pass hand/board
	bool r_hand = false, r_board = false;
	for( int i = 1; i < argc; i++ ) {
		if( strcmp( argv[i], "-h"     ) == 0 ||
		    strcmp( argv[i], "--hand" ) == 0    ) {
			r_hand = true;
			if( i+1 < argc and argv[i+1][0] != '-' ) {
				in_hand = argv[i+1];
				in_hand = add_spaces( in_hand );
			}
		}
		if( strcmp( argv[i], "-b"      ) == 0 ||
		    strcmp( argv[i], "--board" ) == 0    ) {
			r_board = true;
			if( i+1 < argc and argv[i+1][0] != '-' ) {
				in_board = argv[i+1];
				in_board = add_spaces( in_board );
			}
		}
		if( strcmp( argv[i], "--off"   ) == 0 )  off = true;
	}

	cout <<"\n POKER_STATS\n=============\n";

	// Get hand and board
	if( !r_hand ) {
		cout << " Enter HAND : ";
		getline(cin,in_hand);
	}
	if( !r_board ) {
		cout << " Enter BOARD : ";
		getline(cin,in_board);
	}
	string read_cards = in_hand + in_board;

	// Integer representation of playing cards
	vector<int> playing_board;

	// Convert read_cards to playing cards
	ierr = convert_in( read_cards, playing_board );
	if( ierr != 0 ) return 0;

	// Find current showdown
	int score[2];
	calculate( playing_board, score );
#ifdef DEBUG
	cout << "Current Score: " << score[0] << "  " << score[1] << "\n\n";
#endif

	// Calculate drawing odds
	if( !off ) odds( playing_board );
	
	return 0;
}
