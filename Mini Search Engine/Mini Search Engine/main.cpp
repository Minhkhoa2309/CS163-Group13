#include "main.h"

using namespace std;

int main() {
	cout << "Welcome to the mini search engine! " << endl;
	string s;
	Trie* root = new Trie;
	History_Trie* history_root = new History_Trie;
	vector<string> queryHistory, inputSearch;
	string article[11268];
	int start = clock();
	Load_data(root, article);
	int stop = clock();
	cout << "After: " << stop - start << "ms" << endl;
	cout << "Finished Loading!" << endl;
	/*s = History_suggestion(history_root);
	inputSearch = input(queryHistory, s);
	if (queryHistory.size() > 0)
		cout << "History Search: " << endl;
	for (int i = 0; i < queryHistory.size(); i++)
		cout << queryHistory[i] << endl; */
	return 0;
}