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
	cout << "				--------------------------------------------		------------------------------------     " << endl;
	cout << "	 __  __   ___   _   _   ___     ____    _____      _      ____      ____   _   _     _____   _   _    ____   ___   _   _   _____  " << endl;
	cout << "	|  \/  | |_ _| | \ | | |_ _|   / ___|  | ____|    / \    | _  \    / ___| | | | |   | ____| | \ | |  / ___| |_ _| | \ | | | ____| " << endl;
	cout << "	| |\/| |  | |  |  \| |  | |    \___ \  | _|      / _ \   | |_) |   | |    | |_| |   |  _|   |  \| | | |  _   | |  |  \| | |  _|	  " << endl;
	cout << "	| |  | |  | |  | |\  |  | |     ___) | | |___   / ___ \  | _   <   | |___ |  _  |   | |___  | |\  | | |_| |  | |  | |\  | | |___  " << endl;
	cout << "	|_|  |_| |___| |_| \_| |___|   |____/  |_____| /_ / \ _\ | _ |\_\  \____| |_| |_|   |_____| |_| \_|  \____| |___| |_| \_| |_____| " << endl;
		cout << "Query types and guidelines " << endl;
	cout << "-->AND query : results will only be generated if they have both the key words. Ex: keyword1 AND keyword2 " << endl;
	cout << "-->OR query : results will be generated if they have 1 of 2 keywords. Ex: keyword1 OR keyword2 " << endl;
	cout << "-->Intile query : Search for articles that have the keyword in the titles. Ex: Intitle: SpiderMan  " << endl;
	cout << "-->'$' query : Search for articles related to prices. Ex: Shoes $39" << endl;
	cout << "-->'#' query : Search for articles related to Hashtag. Ex: #AI" << endl;
	cout << "-->'$..$ query: Search for articles related to prices within a range. Ex: Watch 200$..300$" << endl;
	cout << "-->'*' query : search for articles with that have the specific keyword. Ex: *Bird" << endl;
	

	/*s = History_suggestion(history_root);
	inputSearch = input(queryHistory, s);
	if (queryHistory.size() > 0)
		cout << "History Search: " << endl;
	for (int i = 0; i < queryHistory.size(); i++)
		cout << queryHistory[i] << endl; */

	return 0;
}