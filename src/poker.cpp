#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include "classes.h"

string myid = "***";

int reorder( vector<int>&, int );
void calc_pot_odds( string );
void assign_arguments( Hand, string&, string& );

int main(int argc, char *argv[]) {

	// Command line argument to call poker_odds program for player myid
	bool poker_odds = false;
	string pass_hand, pass_board;
	for( int i = 1; i < argc; i++ ) {
		if( strcmp( argv[i], "--odds" ) == 0 ||
		    strcmp( argv[i], "-o"      ) == 0 ) {
			poker_odds = true;
		}
		else {
			cout << " Argument " << argv[i] << " not recognized!\n";
			return 0;
		}
	}

	// Class that stores current hand info
	Hand hand;
	hand.reset( 0 );

	// Setup table class by asking for player identification
	Table table;
	table.setup_table();

	// n_order is a vector of seat numbers that tracks live hands
	// Reload a new n_order relative to the button
	vector<int> n_order;
	table.reload( n_order, hand.button );

	// Search temporary
	int seat_number;

	TOP:while( true ) {

		// Increment pre-flop, flop, turn, river
		hand.round++;
		hand.n_action = 0;

		// Round announcement
		table.round_announcer( hand );

		// Reset seen action
		for( auto &n : table.seat ) { n.seen = false; }

		// Reorder whole table utg 
		int utg = table.reutg( n_order, hand );
		reorder( n_order, utg );

		// Note: vector n is reordered at every raise
		vector<int>::iterator n = n_order.begin();

		// Action loop
		while( n != n_order.end() ) {

			// Print seat stats
			cout << table.seat[*n].id << "\n";
			table.seat[*n].stats_out( hand.round );

			// POKER_ODDS program calculates odds for players hand
			// ***NOT currently working/fully implemented!
			if( poker_odds && table.seat[*n].id == myid && !table.seat[*n].seen ) {
				ODDS:char call[30];
				assign_arguments( hand, pass_hand, pass_board );
				sprintf( call, "./poker_odds -h %s -b %s",
					 pass_hand.c_str(), pass_board.c_str() );
				//cout << "call :\n" << call << "\n";
				system( (char *) call );
			}


			// Read ANY user options
			int ierr = 1;
			while( ierr != 0 ) {

				// Read from user
				string action;
				cout << table.seat[*n].id << " ACTION:  ";
				getline( cin>>ws, action );
				cout << "\n";

				// Remove spaces and lowercase
				action.erase(remove(action.begin(), action.end(), ' '), action.end());
			        transform(action.begin(), action.end(), action.begin(),
				          [](unsigned char c){ return tolower(c); });

				// ------------

				// Exit program
				if(      action.find("help" ) != string::npos ) {
					cout << " OPTIONS:\n";
					cout << "==========\n";
					cout << " EXIT/QUIT\n";
					cout << " PRINT     - Output Player (ID)\n";
					cout << " TOP       - New Hand with Button Player (ID)\n";
					cout << " NEXT      - Next action\n";
					cout << " NEW       - New table\n";
					cout << " ADD       - Add  Player (ID)\n";
					cout << " KILL      - KILL Player (ID)\n\n";
				}
				// Exit program
				else if( action.find("exit" ) != string::npos ||
				         action.find("quit" ) != string::npos ) {
					int i_myid = table.search( myid );
					if( i_myid != -1 ) { table.seat[i_myid].write_stats(); }
					return 0;
				}

				// Print all table info
				else if( action.find("print") != string::npos ) {
					seat_number = table.search( action );
					if( seat_number != -1 ) {
						table.seat[seat_number].print_stats();
					}
					else {
						system("clear");
						for( Player it : table.seat ) {
							it.print_stats();
						}
					}
				}

				// Specify button player on new hand
				else if( action.find("top"  ) != string::npos ) {
					seat_number = table.search( action );
					if( seat_number != -1 ) {
						hand.reset( seat_number );
						table.reload( n_order, hand.button );
						goto TOP;
					}
				}

				// Skip to NEXT round
				else if( action.find("next" ) != string::npos ) {
					goto TOP;
				}

				// NEW table
				else if( action.find("new"  ) != string::npos ) {
					hand.reset( 0 );
					table.setup_table();
					table.reload( n_order, hand.button );
					goto TOP;
				}

				// ADD player
				else if( action.find("add"  ) != string::npos ) {
					string read, player_id;
					cout << " Enter new player id: ";
					getline( cin >> ws, read );
					cout << " Enter player they act after: ";
					getline( cin >> ws, player_id );
					seat_number = table.search( player_id );
					if( seat_number != -1 ) {
						table.new_seat( read, seat_number+1 );
						cout << " Going to top...\n";
						hand.reset( hand.button );
						table.reload( n_order, hand.button );
						goto TOP;
					}
				}

				// KILL player
				else if( action.find("kill" ) != string::npos ) {
					seat_number = table.search( action );
					while( seat_number == -1 ) {
						string player_id;
						cout << " Enter player id to kill: ";
						getline( cin >> ws, player_id );
						seat_number = table.search( player_id );
					}
					table.kill( seat_number );
					cout << " Going to top...\n";
					hand.reset( hand.button );
					table.reload( n_order, hand.button );
					goto TOP;
				}

				else if( action.find("odds" ) != string::npos ) {
					goto ODDS;
				}

				// Pot odds calculator ( read BET [into] POT )
				else if( isdigit(action[0]) != 0 ) {
					calc_pot_odds( action );
				}

				// ASSIGN player action and update hand
				else {
					int act = 0;
					if( hand.n_action < 3 ) {
						act = table.seat[*n].assign_stats( action, hand, *n );
					}
					if( act != -99 ) { ierr = 0; }

					// Fold
					if( act == -1 ) {
						n = n_order.erase(n);
						n--;
						if( n_order.size() == 1 ) goto TOP;
					}

					// Raise requires appending all players to act behind
					else if( act == 1 ) {
						n -= reorder( n_order, *n );
					}

				}

				// ------------ Interpret READ

			} // Loop READ error
			
			// Increment hot seat
			n++;

		} // Table round

		// Showdown
		if( hand.round == 3 and n_order.size() > 1 ) {
			table.assign_sd( n_order );
		}

		// Increment hands dealt
		if( hand.round == 1 ) { 
			for( auto &n : table.seat ) { n.hands_inc(); }
		}

		// Hand RESET
		if( n_order.size() < 2 || hand.round > 2 ) {
			system("clear");
			cout << " HAND FINISHED!!\n";
			int button = (hand.button + 1) % table.seat.size();
			hand.reset( button );
			table.reload( n_order, hand.button );
		}

	} // TRUE

	return 0;
}


//  Reorder seat vector on raise/reset
int reorder( vector<int> &n_order, int n_stop ) {
	int counter = 0;
	while( n_order[0] != n_stop ) {
		rotate( n_order.begin(), n_order.begin() + 1, n_order.end() );
		counter++;
		if( counter == n_order.size() ) {
			cout << " ERROR in reorder!!!!!!\n";
			return 1;
		}
	}
	return counter;
}

// Pot odds 
void calc_pot_odds( string read ) {
	cout << "POT_ODDS ";
	int bet, pot, ierr = 0;
	stringstream stream( read );
	stream >> bet;
	if( !stream ) { ierr = 1; }
	stream >> pot;
	if( !stream ) { ierr = 1; }

	if( ierr == 1 ) { cout << "ERROR\n"; return; }

	float odds = 100*bet/(pot+2*bet);
	cout << odds << "%\n";
}

// Command line arguments passed to POKER_STATS
// READ [ AKQJT..32 ] [ SHCD ] ...
void assign_arguments( Hand hand, string &pass_hand, string &pass_board ) {
	string read;
	switch( hand.round ) {
	case 1:
		pass_hand .clear();
		pass_board.clear();

		cout << " Enter HAND: ";
		getline( cin>>ws, read );
		read.erase(remove(read.begin(), read.end(), ' '), read.end());
		for( int i = 0; i < 4; i++ ) {
			pass_hand[i] = read[i];
		}
	break;
	case 2:
		cout << " Enter FLOP: ";
		getline( cin>>ws, read );
		read.erase(remove(read.begin(), read.end(), ' '), read.end());
		for( int i = 0; i < 6; i++ ) {
			pass_board[i] = read[i];
		}
	break;
	case 3:
		cout << " Enter TURN: ";
		getline( cin>>ws, read );
		read.erase(remove(read.begin(), read.end(), ' '), read.end());
		for( int i = 0; i < 2; i++ ) {
			pass_board[i+6] = read[i];
		}
	break;
	case 4:
		cout << " Enter RIVER: ";
		getline( cin>>ws, read );
		read.erase(remove(read.begin(), read.end(), ' '), read.end());
		for( int i = 0; i < 2; i++ ) {
			pass_board[i+8] = read[i];
		}
	break;
	}
	return;
}
