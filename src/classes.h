#ifndef classes
#define classes

#include <string>
#include <vector>
using namespace std;

class Hand {
public:
	int button, round, n_action, last_agg;
	bool cbet;
	void reset( int );
	Hand() { reset(0); }
};

class Stats {
	int hands, vpip, rfi, bet3, f_bet3, c_bet3, flops_seen,
	    agg_flops_seen, ccbet, fcbet, cbet, cbet_turn, turns_seen,
	    w$sd, wtsd;
public:
	bool seen;
	int  assign_stats( string, Hand&, int );
	void hands_inc();
	void  w$sd_inc();
	void  wtsd_inc();
	void stats_out( int );
};

class Player : public Stats {
public:
	string id;
	void print_stats();
	void write_stats();

	Player();
};

class Table {
	vector<Player> storage;
public:
	vector<Player> seat;
	void setup_table();
	void new_table( int );
	void new_seat( string, int );
	void reload( vector<int>&, int  );
	void round_announcer( Hand );
	int  reutg(  vector<int> , Hand );
	int  search( string );
	void kill( int );
	void assign_sd( vector<int> );

	Table();
};

#endif
