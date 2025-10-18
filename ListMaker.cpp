#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
using namespace std ;
int main () {
	string s ;
	ifstream fin ("List.txt") ;
	ofstream fout ("newlist.txt") ;
	while (getline (fin , s)) {
		int len = s.size () ;
		string nw = "" ; bool fl = 0 ;
		for (int i = 0 ; i < len ; i++) {
			if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || s[i] == '-' || s[i] == '\'' || s[i] == '.')
				nw += s[i] , fl = 1 ;
			else {
				if (!fl) continue ;
				else break ;
			}
		}
		fout << nw << endl ;
	}
	return 0 ;
}
