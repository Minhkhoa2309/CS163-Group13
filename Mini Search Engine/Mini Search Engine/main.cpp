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
	cout << "-------------------------------------		------------------------------------     " << endl;
	cout << "----------------------------------MINI - SEARCH ENGINE!!!!!!---------------------------" << endl;
	cout << "-------------------------------------		------------------------------------     " << endl;
	cout << "Query types and guidelines " << endl;
	cout << "-->AND query : results will only be generated if they have both the key words. Ex: keyword1 AND keyword2 " << endl;
	cout << "-->OR query : results will be generated if they have 1 of 2 keywords. Ex: keyword1 OR keyword2 " << endl;
	cout << "-->Intile query : Search for articles that have the keyword in the titles. Ex: Intitle: SpiderMan  " << endl;
	cout << "-->'$' query : Search for articles related to prices. Ex: Shoes $39" << endl;
	cout << "-->'#' query : Search for articles related to Hashtag. Ex: #AI" << endl;
	cout << "-->'$..$ query: Search for articles related to prices within a range. Ex: Watch 200$..300$" << endl;
	cout << "-->'*' query : search for articles with that have the specific keyword. Ex: *Bird" << endl;
	cout << "Please read carefully! " << endl;
	
	system("PAUSE");
	system("CLS");
	while (s != "EXIT")
	{
		s = History_suggestion(history_root);
		inputSearch = input(queryHistory, s);
		if (s == "EXIT")
		{
			break;
		}
		system("CLS");
		callQuery(inputSearch, root, article, s);
		int sz = queryHistory.size();
		if (sz > 0) cout << "History:" << endl;
		for (int i = 0; i < sz; i++)
		{
			cout << queryHistory[i] << endl;
		}
		//system("PAUSE");
		cout << endl;
		system("CLS");

	}
	cout << "Goodbye" << endl;
	destructor(root);
	destructor_History_Trie(history_root);
	cout << endl;

	

	return 0;
}