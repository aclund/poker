#include "functions.h"

string add_spaces( string read ) {
	string str = "";
	for( int i = 0; i < read.length(); i++ ) {
		str += read[i];
		if( i % 2 == 1 ) str += " ";
	}
	return str;
}
