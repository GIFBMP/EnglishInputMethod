#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
using namespace std ;
namespace AutoMaton {
	const int kMaxn = 1e6 + 10 , kMaxOptions = 5 ;
	struct Node {
		string exist_word ;
		int son[35] , visit_count , edge_typ[35] , fail ;
		/*
		edge_typ: 0:not_fail 1:fail
		*/
		Node () {exist_word = "" ; visit_count = 0 ; edge_typ[28] = 1 ;}
	} tree[kMaxn] ;
	bool cmp (int x , int y) {
		if (tree[x].visit_count == tree[y].visit_count) return x < y ;
		else return tree[x].visit_count > tree[y].visit_count ;
	}
	int rt = 0 , total_nodes = 0 ;
	//ignore the difference of capital and small letters
	int GetNum (char ch) {
		if (ch >= 'A' && ch <= 'Z') return ch - 'A' ;
		if (ch >= 'a' && ch <= 'z') return ch - 'a' ;
		if (ch == '.') return 26 ;
		if (ch == '-') return 27 ;
		if (ch == ' ') return 28 ;//return to root
		return ((int)ch) + 100 ;
	}
	void SearchPossibleWords (int x , int begin , int start_id) {
		static int cnt = 0 ;
		if (x == begin) cnt = start_id ;
		if (cnt >= kMaxOptions) return ;
		if (!(tree[x].exist_word).empty ()) {
			cnt++ ;
			cout << cnt << ". " << (tree[x].exist_word) << ' ' ;
		}
		if (cnt >= kMaxOptions) return ;
		int id[35] ;
		for (int i = 0 ; i <= 27 ; i++)
			if (!tree[x].edge_typ[i]) id[i] = tree[x].son[i] ;
			else id[i] = 0 ;
		sort (id , id + 28 , cmp) ;//do not take space into consideration
		for (int i = 0 ; i <= 27 ; i++) {
			if (id[i])
				SearchPossibleWords (id[i] , begin , start_id) ;
		}
	}
	void InsertWord (string s , bool typ = 0) {
		int len = s.size () ;
		int nw = rt ;
		for (int i = 0 ; i < len ; i++) {
			char ch = s[i] ; int x = GetNum (ch) ;
			if (!tree[nw].son[x] || tree[nw].edge_typ[x])
				tree[nw].son[x] = ++total_nodes , tree[nw].edge_typ[x] = 0 ;
			nw = tree[nw].son[x] ;
			if (typ) tree[nw].visit_count++ ;
		}
		tree[nw].exist_word = s ;
	}
	void GetList (string s) {
		int len = s.size () ;
		int nw = rt ; bool fl = 0 ;
		//1:user_input 2:possible_sentence 3~kMaxOptions:possible_first_word
		puts ("--------------------------") ;
		cout << "1. " << s << ' ' ;
		string t = "" ;
		for (int i = 0 ; i < len ; i++) {
			char ch = s[i] ; int x = GetNum (ch) ;
			if (x > 28) {
				fl = 1 ;
				break ;
			}
			if (tree[nw].edge_typ[x]) {
				if (tree[nw].exist_word == "") {
					fl = 1 ;
					break ;
				}
				t += ' ' ;
			}
			if (s[i] != ' ') t += s[i] ;
			nw = tree[nw].son[x] ;
		}
		if (fl) {
			puts ("") ;
			puts ("--------------------------") ;
			return ;
		}
		int start_id = 1 ;
		if (t != s) {
			start_id++ ;
			cout << "2. " << t << ' ' ;
		}
		nw = rt ;
		for (int i = 0 ; i < len ; i++) {
			char ch = s[i] ; int x = GetNum (ch) ;
			if (x > 27 || (tree[nw].edge_typ[x] || !tree[nw].son[x])) {
				fl = 1 ;
				break ;
			}
			nw = tree[nw].son[x] ;
		}
		if (!fl) SearchPossibleWords (nw , nw , start_id) ;
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
	int q[kMaxn] , hd , tl ; 
	void GetFail () {
		hd = 1 ; tl = 0 ;
		for (int i = 0 ; i <= 27 ; i++)
			if (tree[rt].son[i]) q[++tl] = tree[rt].son[i] ;
		while (hd <= tl) {
			int x = q[hd++] ;
			for (int i = 0 ; i <= 27 ; i++) {
				int v = tree[x].son[i] ;
				if (v && !tree[x].edge_typ[i]) tree[v].fail = tree[tree[x].fail].son[i] , q[++tl] = v ;
				else tree[x].son[i] = tree[tree[x].fail].son[i] , tree[x].edge_typ[i] = 1 ;
			}
		}
	}
	void InsertSentence (string s) {
		int len = s.size () ;
		string t = "" ; bool can_insert = 1 ;
		for (int i = 0 ; i < len ; i++) {
			if (s[i] == ' ') {
				if (can_insert) InsertWord (t , 1) ;
				t = "" ; can_insert = 1 ;
				continue ;
			}
			int x = GetNum (s[i]) ;
			if (x > 27) {
				can_insert = 0 ;
				continue ;
			}
			t += s[i] ;
		}
		if (can_insert) InsertWord (t , 1) ;
	}
	
}
using namespace AutoMaton ;
int main () {
	cout << "请输入模式：(0:重置，1：读取记忆)\n" ;
	int typ ; cin >> typ ;
	if (!typ) ResetList () ;
	else ReadList () ;
	GetFail () ;
	cout << "输入法启动，输入 @ESC 退出，输入 @REFRESH 刷新词库\n" ;
	string s ; getline (cin , s) ;
	while (1) {
		getline (cin , s) ;
		if (s == "") continue ;
		//cout << s << endl ;
		if (s == "@ESC") break ;
		if (s == "@REFRESH") {
			GetFail () ;
			continue ;
		}
		GetList (s) ;
		InsertSentence (s) ;
	}
	ofstream fout ("tree.txt") ;
	for (int i = 0 ; i <= total_nodes ; i++) {
		fout << i << ' ' << tree[i].visit_count << ' ' ;
		for (int j = 0 ; j <= 27 ; j++)
			if (!tree[i].edge_typ[j]) fout << tree[i].son[j] << ' ' ;
			else fout << 0 << ' ' ;
		if (tree[i].exist_word == "") fout << 0 << endl ;
		else fout << 1 << ' ' << tree[i].exist_word << endl ;
	}
	return 0 ;
}
