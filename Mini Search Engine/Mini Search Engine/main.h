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

//Template Queue to use data structure unique
template<class QT>
struct QNode
{
	QT* node;
	QNode* next;
	QNode* prev;
};

template<class QT>
class Queue
{
	QNode<QT>* first;
	QNode<QT>* last;
public:
	Queue() {
		first = last = nullptr;
	}
	bool Empty() {
		if (first == last && last == nullptr)
			return true;
		return false;
	}

	void Push_back(QT* x) {
		if (first == nullptr) {
			first = last = new QNode<QT>;
			first->node = x;
			first->next = first->prev = nullptr;
		}
		else {
			last->next = new QNode<QT>;
			last->next->prev = last;
			last->next->next = nullptr;
			last = last->next;
			last->node = x;
		}
	}

	QT* Pop_front() {
		if (first == last && last == nullptr)
			return nullptr;
		else if (first == last) {
			QT* res = first->node;
			delete first;
			first = last = nullptr;
			return res;
		}
		else {
			QT* res = last->node;
			QNode<QT>* tmp = last;
			last = last->prev;
			delete tmp;
			last->next = nullptr;
			return res;
		}
	}
};

void destructor(Trie*& root);
void destructor_History_Trie(History_Trie* root);
void insert_word(Trie*& root, bool intitle, string s, int posinart, int article);
vector<string> ParseStream(string& line, vector<int>& posinart, int& linestart);
void Load_data(Trie* pHead, string article[]);
vector<string> input(vector<string>& queryHistory, string& s);
//void output(vector<int>& res, int stime, string article[], string search_string);
bool checkSpecialCharactor(char s, string tmp); // check special charactors

//Queries
void IntitleQuery(vector<int>& res, Trie* word);
void AndQuery(vector<int>& res, Trie* word);
void queryMinus(vector<int>& res, Trie* word);
Trie* KeyWord(Trie* root, string s);
void queryOr(vector<int>& res, Trie* word);
Trie* hashtag(Trie* root, string str);
Trie* searchMoney(Trie* root, string s);
vector<int> searchMoneyInRange(Trie* root, string s);
vector<int> searchNumberInRange(Trie* root, string s);

//History
string History_suggestion(History_Trie* history_trie);
vector<pair<int, int>> searchIncompleteMatch(Trie* root, string s);
void historyInsert(History_Trie* history_root, string s);
vector<string> historySearch(History_Trie* history_root, string s);
void updateHistory(vector<string>& queryHistory, string s);

//Function to check and execute specific query
void callQuery(vector<string> call, Trie* root, string article[], string search_string);

#endif