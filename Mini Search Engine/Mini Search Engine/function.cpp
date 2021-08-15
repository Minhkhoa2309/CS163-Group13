#include "main.h"

//------------------------------Constructor----------------------------
Trie::Trie()
{
	for (int i = 0; i < 38; i++)
		pNext[i] = NULL;
}

History_Trie::History_Trie()
{
	for (int i = 0; i < 39; i++)
		pNext[i] = NULL;
}
//-------------------------------------------------------------------
//-------------------------------Destructor--------------------------
void destructor(Trie*& root)
{
	if (root == NULL)
		return;
	root->title.clear();
	root->position.clear();
	for (int i = 0; i < 38; i++)
	{
		destructor(root->pNext[i]);
		root->pNext[i] = NULL;
	}
	delete root;
}

void destructor_History_Trie(History_Trie* root)
{
	if (root == NULL)
		return;
	for (int i = 0; i < 39; i++)
	{
		destructor_History_Trie(root->pNext[i]);
		root->pNext[i] = NULL;
	}
	root->exist.clear();
	delete root;
	
}
//-------------------------------------------------------------------

vector<string> ParseStream(string& line, vector<int>& posinart, int& linestart)
{
	transform(line.begin(), line.end(), line.begin(), ::tolower); // convert string to lowercase
	size_t prev = 0, pos; // maintain position in string
	vector<string> wordVector;
	string tmp;
	size_t x;
	while ((pos = line.find_first_of(" \t\n\"`~!@%^&*()-_=+|{}[];:,'<.>/\\?", prev)) != string::npos)
	{
		if (pos > prev)
		{
			tmp = line.substr(prev, pos - prev);
			wordVector.push_back(tmp);
			posinart.push_back(prev + linestart);
		}
		prev = pos + 1;
	}
	x = line.length();
	if (prev < x)
	{
		tmp = line.substr(prev, string::npos);
		wordVector.push_back(tmp);
		posinart.push_back(prev + linestart);
	}
	linestart += (x + 1);
	return wordVector;
}

void insert_word(Trie*& root, bool intitle, string s, int posinart, int article) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	if (s.length() == 0)
		return;
	int tmp, length, cnt;
	Trie* cur = root;
	length = s.length();
	cnt = 0;
	while (s[cnt] != '#' && s[cnt] != '$' && (s[cnt] < 'a' || s[cnt]>'z') && (s[cnt] < 'A' || s[cnt] < 'Z') && (s[cnt] < '0' || s[cnt]>'9'))
	{
		cnt++;
		if (cnt >= length)
			return;
	}
	if (s[cnt] == '#')
	{
		if (cur->pNext[36] == NULL)
			cur->pNext[36] = new Trie;
		cur = cur->pNext[36];
		cnt++;
	}
	else if (s[cnt] == '$')
	{
		if (cur->pNext[37] == NULL)
			cur->pNext[37] = new Trie;
		cur = cur->pNext[37];
		cnt++;
	}
	while (cnt < length) {
		if ((int)(s[cnt] - '0') >= 0 && (int)(s[cnt] - '0') <= 9) {
			tmp = (int)(s[cnt] - '0');
			if (cur->pNext[tmp + 26] == NULL)
				cur->pNext[tmp + 26] = new Trie;
			cur = cur->pNext[tmp + 26];
		}
		else if ((int)(s[cnt] - 'a') >= 0 && (int)(s[cnt] - 'a') <= (int)('z' - 'a'))
		{
			tmp = (int)(s[cnt] - 'a');
			if (cur->pNext[tmp] == NULL)
				cur->pNext[tmp] = new Trie;
			cur = cur->pNext[tmp];
		}
		cnt++;
	}
	if (intitle)cur->title.push_back(article);
	cur->position.push_back(make_pair(article, posinart));
	return;
}



void Load_data(Trie* pHead, string article[])
{
	cout << "Loading data................";
	ifstream fin1, fin2;
	string filename;
	string line;
	int i, linestart, cnt;
	int index_of_article, check, word_vector_size;
	vector<string> word_vector;
	vector<int> position_vector;
	fin1.open ("Search Engine-Data/___index.txt");
	i = 0;
	if (!fin1.is_open())
		cout << "Can't open this file" << endl;
	while (!fin1.eof())
	{
		getline(fin1, filename);
		string sub = "Search Engine-Data/";
		sub += filename;
		fin2.open(sub);
		if (fin2.fail())
		{
			continue;
		}
		getline(fin2, line);
		article[i] = line;
		linestart = 0;
		word_vector = ParseStream(line, position_vector, linestart);
		index_of_article = i;
		article[index_of_article] += '\n';
		word_vector_size = word_vector.size();
		for (cnt = 0; cnt < word_vector_size; cnt++)
		{
			insert_word(pHead, true, word_vector[cnt], position_vector[cnt], index_of_article);
		}
		while (!fin2.eof()) {
			getline(fin2, line);
			article[i] += line + '\n';
			word_vector.clear();
			position_vector.clear();
			word_vector = ParseStream(line, position_vector, linestart);
			word_vector_size = word_vector.size();
			for (cnt = 0; cnt < word_vector_size; cnt++)
			{
				insert_word(pHead, false, word_vector[cnt], position_vector[cnt], index_of_article);
			}
		}
		fin2.close();
		i++;
	}
	fin1.close();
}

void IntitleQuery(vector<int>& res, Trie* word)
{
	if (word == NULL)
		return;
	vector<int> first = res;
	vector<int> second, des;
	second = word->title;
	if (res.size() <= 0)
	{
		res = second;
		return;
	}
	set_intersection(first.begin(), first.end(), second.begin(), second.end(),back_inserter(des));
	res = des;
}


//Or Query
void queryOr(vector<int>& res, Trie* word) {
	if (word == NULL)
		return;

	vector<int> second, destination;
	size_t size = word->position.size();

	for (size_t i = 0; i < size; i++) {
		second.push_back(word->position[i].first);
	}

	set_union(res.begin(), res.end(), second.begin(), second.end(), back_inserter(destination));
	res = destination;
}

//And Query
void AndQuery(vector<int>& res, Trie* word) {
	if (word == NULL)
		return;
	vector<int> second, des;
	size_t sz = word->position.size();
	for (size_t i = 0; i < sz; i++) {
		second.push_back(word->position[i].first);
	}
	set_intersection(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
	res = des;
}

//Hashtag search
Trie* hashtag(Trie* root, string str) {
	//Set all words to lowercase
	transform(str.begin(), str.end(), str.begin(), ::tolower);

	Trie* current = root;
	int length, count, tmp;

	length = str.length();
	count = 1;

	if (current->pNext[36] != NULL)
		current = current->pNext[36];
	else
		return NULL;

	while (count < length) {
		if ((int)(str[count] - '0') < 10 && (int)(str[count] - '0') >= 0)
			tmp = (int)(str[count] - '0') + 26;
		else if ((int)(str[count] - 'a') >= 0 && (int)(str[count] - 'a') <= (int)('z' - 'a'))
			tmp = (int)(str[count] - 'a');
		else {
			return NULL;
			continue;
		}
		if (current->pNext[tmp] == NULL)return NULL;
		current = current->pNext[tmp];
		count++;
	}

	return current;
}
