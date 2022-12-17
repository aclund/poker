#ifndef functions
#define functions

#include <string>
#include <vector>
#include <iostream>
using namespace std;

string add_spaces(string);
void calculate(vector<int>,int[2]);
void combinations(vector<int>,int,int[],int,int,
		  vector<int>,int[9]);
int  convert_in(string,vector<int>&);
void odds(vector<int>);
void preform_calc(vector<int>,int[],int,int[9]);
void quickSort(int[],int,int);

#endif
