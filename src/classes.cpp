#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#include "classes.h"

int Stats::assign_stats( string input, Hand& hand, int p2act ) {

	// Act return: -1=Player Folds  0=No Error  1=Raise  -99=Reading Error
	int act = 0;

	// Convert input to int action;
	int action;
	if(      input[0] == 'f' || input == "fold"  ) {
		action = 0;
		act = -1;
	}
	else if( input[0] == '.' || input == "check" ) {
		action = 1;
	}
	else if( input[0] == 'c' || input == "call"  ) {
		action = 2;
	}
	else if( input[0] == 'r' || input == "raise" ) {
		action = 3;
		act = 1;
	}
	else if( input[0] == 'e' || input == "erron" ) {
		// Erroneous Player previous folded or sitting out
		return -1;
	}
	else {
		cout << " Input NOT recognized!\n";
		return -99;
	}

	switch( hand.round ) {
	case 1: // PRE-FLOP

		// Hand was already 3+ bet
		if( hand.n_action >= 3 ) c_bet3++;

		switch( action ) {
		case 0: // fold
			if( hand.n_action >= 3 ) f_bet3++;
		break;
		case 1: // check
		break;
		case 2: // call
			if( !seen ) { vpip++; }
		break;
		case 3: // raise
			if( !seen ) { vpip++; }
			if(      hand.n_action == 0 ) {  rfi++; }
			else if( hand.n_action >= 2 ) { bet3++; }
			hand.n_action++;
			hand.last_agg = p2act;
		break;
		}

	break;
	case 2: // FLOP

		if( !seen ) { flops_seen++; }
		if( !seen and p2act == hand.last_agg ) { agg_flops_seen++; }

		// Facing C-bet
		if( !seen and hand.cbet ) { ccbet++; }

		switch( action ) {
		case 0: // fold
			if( !seen and hand.cbet ) { fcbet++; }
		break;
		case 1: // check
		break;
		case 2: // call
		break;
		case 3: // raise
			if( !seen and hand.n_action == 0 and p2act == hand.last_agg ) {
				cbet++;
				hand.cbet = true;
			}
			hand.n_action++;
		break;
		}
	break;
	case 3: // TURN

		if( !seen and hand.last_agg  ) { turns_seen++; }

		switch( action ) {
		case 0: // fold
		break;
		case 1: // check
		break;
		case 2: // call
		break;
		case 3: // raise
			if( !seen and hand.n_action == 0 and p2act == hand.last_agg ) {
				cbet_turn++;
			}
			hand.n_action++;
		break;
		}
	break;
	case 4: // RIVER
	break;
	}

	seen = true;

	return act;
}
void Stats::hands_inc() { hands++; }
void Stats:: w$sd_inc() {  w$sd++; }
void Stats:: wtsd_inc() {  wtsd++; }
void Stats::stats_out( int round ) {
	printf("============================================================\n");
	if( round == 1 ) {
		printf(" Hands       VPIP         RFI        3-Bet    Fold to 3-Bet \n");
		printf("------------------------------------------------------------\n");
		if( hands == 0 ) {
			printf("    0\n");
			return;
		}
		printf("%5d %11.2f%% %10.2f%% %10.2f%% %10.2f%% %d/%d \n", hands, 
			float(100*vpip)/float(hands), float(100*   rfi)/float(hands),
			float(100*bet3)/float(hands), float(100*f_bet3)/float(c_bet3+1e-12), f_bet3, c_bet3 );
	}
	else {
		printf("  C-Bet     FC-bet      C-bet-T       WTSD        W$SD      \n");
		printf("------------------------------------------------------------\n");
		printf("%6.2f%% %9.2f%% %10.2f%% %10.2f%% %10.2f%% \n",
			float(100*cbet     )/float(agg_flops_seen+1e-12), float(100*fcbet)/float(ccbet     +1e-12),
			float(100*cbet_turn)/float(agg_flops_seen+1e-12), float(100*wtsd )/float(flops_seen+1e-12),
			float(100*w$sd     )/float(wtsd          +1e-12) );
	}
}

Player::Player() : Stats() {}
void Player::write_stats() {
	freopen("statistics.out","w",stdout);
	stats_out(1);
	stats_out(2);
}
void Player::print_stats() {
	cout << " PLAYER " << id << "\n";
	stats_out(1);
	stats_out(2);
	cout << "\n";
}

Table::Table() {
}
void Table::setup_table() {
	system( "clear" );

	int n_players;
	cout << " Enter number of players at table: ";
	while( !(cin >> n_players) ) {
		cin.clear();
		cin.ignore();
	}

	new_table( n_players );

	cout << " Enter player identifications: (self=***) Starting with BUTTON\n";
	string read;
	for( int n = 0; n < n_players; n++ ) {
		cin >> read;
		transform(read.begin(), read.end(), read.begin(),
		          [](unsigned char c){ return tolower(c); });
		new_seat( read, seat.size() );
	}
}
void Table::new_table( int n_new ) {
	// Put current table into storage and clear table
	storage.insert( storage.end(), seat.begin(), seat.end() );
	seat.clear();
}
void Table::new_seat( string read, int position ) {
	bool found = false;
	vector<Player>::iterator it;
	for( it = storage.begin(); it < storage.end(); it++ ) {
		if( it->id == read ) {
			Player add_seat = *it;
			seat.insert( seat.begin() + position, add_seat );
			it = storage.erase(it);
			found = true;
			break;
		}
	}
	for( it = seat.begin(); it < seat.end(); it++ ) {
		if( it->id == read ) {
			found = true;
			cout << " Ooops player id " << read << " already sitting down!\n";
		}
	}
	if( !found ) {
		Player add_seat;
		add_seat.id = read;
		seat.insert( seat.begin() + position, add_seat );
	}
}
void Table::kill( int seat_number ) {
	storage.insert( storage.end(), seat[seat_number] );
	seat.erase( seat.begin() + seat_number );
}
void Table::reload( vector<int> &n_order, int button ) {
	n_order.clear();

	int utg;
	if( seat.size() > 2 ) {
		utg = (button + 3) % seat.size();
	}
	else {
		utg = button;
	}

	for( int i = 0; i < seat.size(); i++ ) {
		int n = (i + utg) % seat.size();
		n_order.push_back(n);
	}
}
int Table::reutg( vector<int> n_order, Hand hand ) {

	// Original order
	vector<int> n_orig;
	reload( n_orig, hand.button );

	// UTG changes post-flop
	if( hand.round != 1 ) {
		int n_rot = 1;
		if( seat.size() > 2 ) { n_rot = 2; }
		rotate( n_orig.begin(), n_orig.end() - n_rot, n_orig.end() );
	}

	for( vector<int>::iterator it = n_orig.begin(); it < n_orig.end(); it++ ) {
		for( vector<int>::iterator it1 = n_order.begin(); it1 < n_order.end(); it1++ ) {
			if( *it == *it1 ) {
				return *it;
			}
		}
	}

	return 0;
}
int Table::search( string word ) {
	int count = 0;
	for( vector<Player>::iterator it = seat.begin();
				      it < seat.end()  ; it++ ){
		if( word.find(it->id) != string::npos ) {
			return count;
		}
		count++;
	}
	// NOT FOUND
	return -1;
}
void Table::assign_sd( vector<int> n_order ) {
	cout << " SHOWDOWN!\n";
	cout << " Winner: ";

	bool found = false;
	string read;
	while( !found ) {
		cin >> read;
		for( int it : n_order ) {
			if( read.find( seat[it].id ) != string::npos ) {
				seat[it].w$sd_inc();
				found = true;
			}
		}
		if( read.find( "exit" ) != string::npos ) return;
	}

	for( int it : n_order ) {
		seat[it].wtsd_inc();
	}
}
void Table::round_announcer( Hand hand ){

	cout << "\n";

	switch( hand.round ) {
	case 1:
		cout << "  PREFLOP";
	break;
	case 2:
		cout << "  FLOP";
	break;
	case 3:
		cout << "  TURN";
	break;
	case 4:
		cout << "  RIVER";
	break;
	default:
		cout << "ERROR: ANNOUNCMENT?\n";
	break;
	}

	cout << "\n\n";
}

void Hand::reset( int new_button ) {
	button    = new_button;
	round     = 0;
	n_action  = 0;
	last_agg  = -1;
	cbet      = false;
}
