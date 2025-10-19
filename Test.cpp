#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
using namespace std ;
const int kMaxn = 1e6 + 10 , kMaxOptions = 5 ;
struct Node {
	string exist_word ;
	int son[35] ;
	Node () {exist_word = "" ;}
} tree[kMaxn] ;
int rt , total_nodes ;
//ignore the difference of capital and small letters
int GetNum (char ch) {
	if (ch >= 'A' && ch <= 'Z') return ch - 'A' ;
	if (ch >= 'a' && ch <= 'z') return ch - 'a' ;
	if (ch == '.') return 26 ;
	if (ch == '-') return 27 ;
	return (int)ch ;
}
void SearchPossibleWords (int x , int begin) {
	static int cnt = 0 ;
	if (x == begin) cnt = 1 ;
	if (cnt >= kMaxOptions) return ;
	if (!(tree[x].exist_word).empty ()) {
		cnt++ ;
		cout << cnt << ". " << (tree[x].exist_word) << ' ' ;
	}
	if (cnt >= kMaxOptions) return ;
	for (int i = 0 ; i <= 27 ; i++) {
		if (tree[x].son[i])
			SearchPossibleWords (tree[x].son[i] , begin) ;
	}
}
void GetList (string s) {
	int len = s.size () ;
	int nw = rt ; bool fl = 0 ;
	puts ("--------------------------") ;
	cout << "1. " << s << ' ' ;
	for (int i = 0 ; i < len ; i++) {
		char ch = s[i] ; int x = GetNum (ch) ;
		if (x > 27 || !tree[nw].son[x]) {
			fl = 1 ;
			break ;
		}
		nw = tree[nw].son[x] ;
	}
	if (!fl) SearchPossibleWords (nw , nw) ;
	puts ("") ;
	puts ("--------------------------") ;
}
int main () {
	ifstream fin ("newlist.txt") ;
	string s ;
	while (getline (fin , s)) {
		int len = s.size () ;
		int nw = rt ;
		for (int i = 0 ; i < len ; i++) {
			char ch = s[i] ; int x = GetNum (ch) ;
			if (!tree[nw].son[x]) tree[nw].son[x] = ++total_nodes ;
			nw = tree[nw].son[x] ;
		}
		tree[nw].exist_word = s ;
	}
	while (1) {
		cin >> s ;
		GetList (s) ;
	}
	return 0 ;
}
