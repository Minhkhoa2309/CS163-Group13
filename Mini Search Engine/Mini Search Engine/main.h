#ifndef _main_h_
#define _main_h_

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <fstream>
#include <ctime>
#include <queue>

using namespace std;


struct Trie {
	vector<int> title;
	vector<pair<int, int>> position;  //include article, postion in article
	Trie* pNext[38];  //Character a->z, 0->9, #, $
	Trie();
};

struct History_Trie {
	vector<string> exist;
	History_Trie* pNext[39]; //Character a->z, 0->9, #,$, space
	History_Trie();
};

void destructor(Trie*& root);
void destructor_History_Trie(History_Trie* root);
void insert_word(Trie*& root, bool intitle, string s, int posinart, int article);
vector<string> ParseStream(string& line, vector<int>& posinart, int& linestart);
void Load_data(Trie* pHead, string article[]);

#endif