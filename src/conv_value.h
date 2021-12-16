inline int conv_value(char value) {

	if( value == '2' ) return 2;
	if( value == '3' ) return 3;
	if( value == '4' ) return 4;
	if( value == '5' ) return 5;
	if( value == '6' ) return 6;
	if( value == '7' ) return 7;
	if( value == '8' ) return 8;
	if( value == '9' ) return 9;
	if( value == 'T' ) return 10;
	if( value == 'J' ) return 11;
	if( value == 'Q' ) return 12;
	if( value == 'K' ) return 13;
	if( value == 'A' ) return 14;

	return 0;
}
