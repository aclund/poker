PROGRAMS = poker poker_odds

poker: classes.o poker.o
       classes.o poker.o: classes.h

poker_odds: add_spaces.o calculate.o combinations.o convert_in.o quickSort.o odds.o poker_odds.o
            add_spaces.o calculate.o combinations.o convert_in.o quickSort.o odds.o poker_odds.o: functions.h
