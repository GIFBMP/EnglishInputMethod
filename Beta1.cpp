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
	int son[35] , visit_count ;
	Node () {exist_word = "" ; visit_count = 0 ;}
} tree[kMaxn] ;
bool cmp (int x , int y) {
	if (tree[x].visit_count == tree[y].visit_count) return x < y ;
	else return tree[x].visit_count > tree[y].visit_count ;
}
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
	int id[35] ;
	for (int i = 0 ; i <= 27 ; i++) id[i] = tree[x].son[i] ;
	sort (id , id + 28 , cmp) ;
	for (int i = 0 ; i <= 27 ; i++) {
		if (id[i])
			SearchPossibleWords (id[i] , begin) ;
	}
}
void InsertWord (string s , bool typ = 0) {
	int len = s.size () ;
	int nw = rt ;
	for (int i = 0 ; i < len ; i++) {
		char ch = s[i] ; int x = GetNum (ch) ;
		if (!tree[nw].son[x]) tree[nw].son[x] = ++total_nodes ;
		nw = tree[nw].son[x] ;
		if (typ) tree[nw].visit_count++ ;
	}
	tree[nw].exist_word = s ;
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
void ResetList () {
	string s ;
	ifstream fin ("newlist.txt") ;
	while (getline (fin , s)) {
		InsertWord (s , 0) ;
	}
}
void ReadList () {
	string s ; int id ;
	ifstream fin ("tree.txt") ;
	while (!fin.eof ()) {
		fin >> id ;
		//cout << id << endl ;
		total_nodes++ ;
		fin >> tree[id].visit_count ;
		for (int i = 0 ; i <= 27 ; i++)
			fin >> tree[id].son[i] ;
		//cout << endl ;
		int x ; fin >> x ;
		//cout << x << endl ;
		//if (fin.eof ()) cout << "*" ;
		if (x == 1) fin >> tree[id].exist_word ;
	}
	total_nodes-- ;
	if (total_nodes <= 0) {
		cout << "读取错误，已重置状态\n" ;
		ResetList () ;
	}
	//防止意外 
}
int main () {
	cout << "请输入模式：(0:重置，1：读取记忆)\n" ;
	int typ ; cin >> typ ;
	if (!typ) ResetList () ;
	else ReadList () ;
	cout << "输入法启动，输入 @ESC 退出\n" ; 
	while (1) {
		string s ; cin >> s ;
		if (s == "@ESC") break ;
		GetList (s) ;
		InsertWord (s , 1) ;
	}
	ofstream fout ("tree.txt") ;
	for (int i = 0 ; i <= total_nodes ; i++) {
		fout << i << ' ' << tree[i].visit_count << ' ' ;
		for (int j = 0 ; j <= 27 ; j++) fout << tree[i].son[j] << ' ' ;
		if (tree[i].exist_word == "") fout << 0 << endl ;
		else fout << 1 << ' ' << tree[i].exist_word << endl ;
	}
	return 0 ;
}
