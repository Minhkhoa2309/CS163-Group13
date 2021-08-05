#include "main.h"

using namespace std;

int main() {
	cout << "Welcome to the mini search engine! " << endl;
	string s;
	Trie* root = new Trie;
	History_Trie* histryRoot = new History_Trie;
	string article[11268];
	Load_data(root, article);
	cout << endl;
	return 0;
}