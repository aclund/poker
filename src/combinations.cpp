#include "functions.h"

void preform_calc( vector<int> playing_board, int data[], int n_add, int tracker[9] ) {

	// Set temporary board
	vector<int> tmp_board = playing_board;
	for( int i=0; i<n_add; i++ ) {
		tmp_board.push_back(data[i]);
	}

	// Calculate score
	int score[2];
	calculate( tmp_board, score );
#ifdef DEBUG
	cout << "Score: " << score[0] << "  " << score[1] << "\n\n";
#endif

	// Update tracker
	tracker[score[0]]++;

	return;
}

void combinations( vector<int> ex_deck, int i, int data[], int index, int n_add,
		   vector<int> playing_board, int tracker[9] ) {

    // Current combination is ready
    if( index == n_add ) {
    	preform_calc( playing_board, data, n_add, tracker );
        return;
    }

    // When no more elements are there to put in data[]
    if( i >= ex_deck.size() )
        return;

    // current is included, put next at next location
    data[index] = ex_deck[i];
    combinations( ex_deck, i+1, data, index+1, n_add, playing_board, tracker );

    // current is excluded, replace it with next (Note that
    // i+1 is passed, but index is not changed)
    combinations( ex_deck, i+1, data, index  , n_add, playing_board, tracker );
}
