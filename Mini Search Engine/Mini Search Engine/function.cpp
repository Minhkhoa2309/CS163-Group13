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
	cout << "Loading data................" << endl;
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

//check special charactor
bool checkSpecialCharactor(char s, string tmp)
{
	char e[32] = { '+',',',';',' ','\t','\0','!','{','}','"','(',')','\f','\v','\n','\r','=','|','~','@','%', '&','<','>','/','[',']','?','`','^','_','\\' };
	for (int i = 0; i < 32; i++)
		if (s == e[i])
			return false;
	if ((s == ':') && (tmp != "intitle"))
		return false;
	if ((s == '-') && (tmp.empty()))
		return true;
	return true;
}


//Query for search title
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

//Search key word
Trie* KeyWord(Trie* root, string s) {
	vector<pair<int, int> >res;
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	Trie* cur = root;

	int length = s.length();
	int cnt = 0;

	if (s[cnt] == '#') {
		if (cur->pNext[36] == NULL)
			return NULL;
		cur = cur->pNext[36];
		cnt++;
	}
	else if (s[cnt] == '$') {
		if (cur->pNext[37] == NULL)
			return NULL;
		cur = cur->pNext[37];
		cnt++;
	}

	int tmp;
	while (cnt < length) {
		if ((int)(s[cnt] - '0') < 10 && (int)(s[cnt] - '0') >= 0)
			tmp = (int)(s[cnt] - '0') + 26;
		else if ((int)(s[cnt] - 'a') >= 0 && (int)(s[cnt] - 'a') <= (int)('z' - 'a'))
			tmp = (int)(s[cnt] - 'a');
		else {
			cnt++;
			continue;
		}

		if (cur->pNext[tmp] == NULL)
			return NULL;

		cur = cur->pNext[tmp];
		cnt++;
	}
	return cur;
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

/****************************************************************/

Trie* searchMoney(Trie* root, string s)
{
	Trie* cur = root;
	int length = s.length();
	int cnt = 1, tmp;
	if (cur->pNext[37] != NULL)
		cur = cur->pNext[37];
	else
		return NULL;
	while (cnt < length) {
		if ((int)(s[cnt] - '0') < 10 && (int)(s[cnt] - '0') >= 0)
			tmp = (int)(s[cnt] - '0') + 26;
		else {
			return NULL;
			continue;
		}
		if (cur->pNext[tmp] == NULL)return NULL;
		cur = cur->pNext[tmp];
		cnt++;
	}
	return cur;
}

/****************************************************************/

vector<int> searchMoneyInRange(Trie* root, string s)
{
	//check if this is a mistake
	vector<int> res;
	Trie* cur = root;
	if (cur->pNext[37] == NULL)
		return res;
	cur = cur->pNext[37];
	//transform string to int
	int intst = 0;
	int l = s.length();
	if (s[1] < '0' || s[1]>'9')return res;
	int test = 2;
	while (test < l)
	{
		if ((s[test] >= '0' && s[test] <= '9') || (s[test] == '.') || (s[test] == '$'))
			test++;
		else
			return res;
	}
	int cnt = 1;
	while (cnt < l)
	{
		intst *= 10;
		intst += (s[cnt] - '0');
		cnt++;
		if (s[cnt] == '.')
		{
			cnt++;
			break;
		}
	}
	//search from intst to inte
	if (s[cnt] != '.')
		return res;
	cnt++;
	if (s[cnt] != '$')
		return res;
	cnt++;
	int inte = 0;
	while (cnt < l)
	{
		if (s[cnt] == '.' || s[cnt] == '$')
			return res;
		inte *= 10;
		inte += (s[cnt] - '0');
		cnt++;
	}
	if (intst > inte)return res;
	int tmp = intst;
	Trie* nxt;
	string nxts;
	int cnt2;
	while (tmp <= inte)
	{
		nxts = "$";
		test = tmp;
		if (test == 0)cnt = 1;
		else
		{
			cnt = 0;
			while (test != 0)
			{
				cnt++;
				test /= 10;
			}
		}
		test = tmp;
		intst = 0;
		cnt2 = cnt;
		while (cnt2 != 0)
		{
			intst *= 10;
			intst += test % 10;
			test /= 10;
			cnt2--;
		}
		while (cnt != 0)
		{
			nxts += intst % 10 + '0';
			intst /= 10;
			cnt--;
		}
		nxt = searchMoney(root, nxts);
		queryOr(res, nxt);
		tmp++;
	}
	return res;
}

/****************************************************************/

vector<int> searchNumberInRange(Trie* root, string s)
{
	//check if this is a mistake
	int test;
	vector<int> res;
	int l = s.length();
	int cnt = 0;
	int intst = 0, inte = 0;
	if (s[0] == '.')return res;
	while (cnt < l)
	{
		intst *= 10;
		intst += (int)(s[cnt] - '0');
		cnt++;
		if (s[cnt] == '.')
		{
			cnt++;
			break;
		}
	}
	if (s[cnt] != '.')
		return res;
	cnt++;
	if (cnt >= l)return res;
	while (cnt < l)
	{
		if (s[cnt] == '.')
			return res;
		inte *= 10;
		inte += (int)(s[cnt] - '0');
		cnt++;
	}
	if (intst > inte)return res;
	int tmp = intst;
	Trie* nxt;
	string nxts;
	int cnt2;
	while (tmp <= inte)
	{
		nxts.clear();
		test = tmp;
		if (test == 0)cnt = 1;
		else
		{
			cnt = 0;
			while (test != 0)
			{
				cnt++;
				test /= 10;
			}
		}
		test = tmp;
		intst = 0;
		cnt2 = cnt;
		while (cnt2 != 0)
		{
			intst *= 10;
			intst += test % 10;
			test /= 10;
			cnt2--;
		}
		while (cnt != 0)
		{
			nxts += intst % 10 + '0';
			intst /= 10;
			cnt--;
		}
		nxt = KeyWord(root, nxts);
		queryOr(res, nxt);
		tmp++;
	}
	return res;
}

//-----------------------------------History Search -------------------------------------------------------
vector<string> input(vector<string>& queryHistory, string& s)
{
	if (s == "EXIT")return vector<string>();
	updateHistory(queryHistory, s);
	string tmp;
	vector<string> result;
	istringstream iss(s);
	for (string s; iss >> s; )
	{
		int i, l = static_cast<int>(s.length());
		for (i = 0; i < l; i++)
		{
			if (checkSpecialCharactor(s[i], tmp))
				tmp += s[i];
			if (tmp == "intitle:")
				break;
			if (tmp == "-")
				break;
		}
		if ((tmp == "intitle:"))
		{
			result.push_back(tmp);
			tmp.clear();
			for (int i = 8; i < l; i++)
			{
				if (checkSpecialCharactor(s[i], tmp))
					tmp += s[i];
			}
		}
		if ((tmp == "-"))
		{
			result.push_back(tmp);
			tmp.clear();
			for (int i = 1; i < l; i++)
			{
				if (checkSpecialCharactor(s[i], tmp))
					tmp += s[i];
			}
		}
		if ((tmp == "#"))
		{
			result.push_back(tmp);
			tmp.clear();
			for (int i = 1; i < l; i++)
			{
			if (checkSpecialCharactor(s[i], tmp))
			tmp += s[i];
			}
		}
		if (!tmp.empty())
			result.push_back(tmp);
		tmp.clear();
	}
	return result;
}

void updateHistory(vector<string>& queryHistory, string s)
{
	int size = queryHistory.size();
	if (size < 8)
	{
		queryHistory.push_back(s);
	}
	else
	{
		queryHistory.erase(queryHistory.begin());
		queryHistory.push_back(s);
	}
}

void historyInsert(History_Trie* history_root, string s) {
	if (s.length() == 0)
		return;
	int tmp, length, cnt;
	History_Trie* cur = history_root;
	length = s.length();
	cnt = 0;
	while (cnt < length) {
		if ((int)(s[cnt] - '0') >= 0 && (int)(s[cnt] - '0') <= 9) {
			tmp = (int)(s[cnt] - '0');
			if (cur->pNext[tmp + 26] == NULL)
				cur->pNext[tmp + 26] = new History_Trie;
			cur = cur->pNext[tmp + 26];
		}
		else if (s[cnt] >= 'A' && s[cnt] <= 'Z')
		{
			tmp = (int)(s[cnt] - 'A');
			if (cur->pNext[tmp] == NULL)
				cur->pNext[tmp] = new History_Trie;
			cur = cur->pNext[tmp];
		}
		else if ((int)(s[cnt] - 'a') >= 0 && (int)(s[cnt] - 'a') <= (int)('z' - 'a'))
		{
			tmp = (int)(s[cnt] - 'a');
			if (cur->pNext[tmp] == NULL)
				cur->pNext[tmp] = new History_Trie;
			cur = cur->pNext[tmp];
		}
		else if (s[cnt] == '#')
		{
			if (cur->pNext[36] == NULL)
				cur->pNext[36] = new History_Trie;
			cur = cur->pNext[36];
		}
		else if (s[cnt] == '$')
		{
			if (cur->pNext[37] == NULL)
				cur->pNext[37] = new History_Trie;
			cur = cur->pNext[37];
		}
		else if (s[cnt] == ' ') {
			if (cur->pNext[38] == NULL)
				cur->pNext[38] = new History_Trie;
			cur = cur->pNext[38];
		}
		cnt++;
	}
	bool check = true;
	length = cur->exist.size();
	for (cnt = 0; cnt < length; cnt++)
	{
		if (cur->exist[cnt] == s)
		{
			check = false;
			break;
		}
	}
	if (check)
		cur->exist.push_back(s);
	return;
}

vector< string > historySearch(History_Trie* history_root, string s) {
	vector< string > res;
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	History_Trie* cur = history_root;
	int length = s.length(), i = 0;
	for (; i < length; i++) {
		int tmp;
		if ((int)(s[i] - '0') < 10 && (int)(s[i] - '0') >= 0)
			tmp = (int)(s[i] - '0') + 26;
		else if ((int)(s[i] - 'a') >= 0 && (int)(s[i] - 'a') <= (int)('z' - 'a'))
			tmp = (int)(s[i] - 'a');
		else if (s[i] == ' ')
			tmp = 38;
		else if (s[i] == '$')
			tmp = 37;
		else if (s[i] == '#')
			tmp = 36;
		else continue;
		if (cur->pNext[tmp] == NULL)
			return res;
		cur = cur->pNext[tmp];
	}
	if (!(cur->exist).empty())
	{
		length = cur->exist.size();
		for (i = 0; i < length; i++)
			res.push_back(cur->exist[i]);
	}
	Queue<History_Trie> q;
	for (int i = 0; i < 39; i++)
		if (cur->pNext[i] != NULL)
			q.Push_back(cur->pNext[i]);
	while (!q.Empty()) {
		cur = q.Pop_front();
		if (!cur->exist.empty())
		{
			length = cur->exist.size();
			for (i = 0; i < length; i++)
				res.push_back(cur->exist[i]);
		}
		for (int i = 0; i < 39; i++)
			if (cur->pNext[i] != NULL)
				q.Push_back(cur->pNext[i]);
	}
	return res;
}

vector<pair<int, int>> searchIncompleteMatch(Trie* root, string s)
{
	vector< pair<int, int> > res;
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	Trie* cur = root;
	int length = s.length();
	if (s[0] == '#')
	{
		if (cur->pNext[36] == NULL)
			return vector<pair<int, int>>();
		cur = cur->pNext[36];
	}
	else if (s[0] == '$')
	{
		if (cur->pNext[37] == NULL)
			return vector<pair<int, int>>();
		cur = cur->pNext[37];
	}
	int tmp;
	for (int cnt = 0; cnt < length; cnt++) {
		if ((int)(s[cnt] - '0') < 10 && (int)(s[cnt] - '0') >= 0)
			tmp = (int)(s[cnt] - '0') + 26;
		else if ((int)(s[cnt] - 'a') >= 0 && (int)(s[cnt] - 'a') <= (int)('z' - 'a'))
			tmp = (int)(s[cnt] - 'a');
		else continue;
		if (cur->pNext[tmp] == NULL)
			return res;
		cur = cur->pNext[tmp];
	}
	if (!(cur->position).empty())
		res = cur->position;
	queue<Trie*> q;
	for (int i = 0; i < 38; i++)
		if (cur->pNext[i] != NULL)
			q.push(cur->pNext[i]);
	while (!q.empty()) {
		cur = q.front();
		q.pop();
		if (!cur->position.empty())
			res.insert(res.end(), cur->position.begin(), cur->position.end());
		for (int i = 0; i < 38; i++)
			if (cur->pNext[i] != NULL)
				q.push(cur->pNext[i]);
	}
	return res;
}

string History_suggestion(History_Trie* history_root) {
	HANDLE h_std_out;
	COORD coord;
	h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int x, y;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	vector< string > res;
	string search_string = "";
	string tmp;
	char ch;
	int cnt = 0, cnt_ch = 0;

	ch = _getch();
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		x = csbi.dwCursorPosition.X;
		y = csbi.dwCursorPosition.Y;
	}
	string dbg;
	while (ch != 13) {
		if (ch == '\b')
		{
			dbg.clear();
			dbg = search_string.substr(0, search_string.length() - 1);
			search_string.clear();
			search_string = dbg;
			cnt_ch -= 2;
		}
		else
			search_string.push_back(ch);
		system("CLS");
		cout << search_string;
		res.clear();
		res = historySearch(history_root, search_string);
		coord.X = 0;
		coord.Y = y + cnt + 1;
		SetConsoleCursorPosition(h_std_out, coord);
		while (!res.empty()) {
			tmp = res.back();
			res.pop_back();
			cout << tmp << endl;
		}
		++cnt_ch;
		//ch = _getch();
		coord.X = cnt_ch;
		coord.Y = y;
		SetConsoleCursorPosition(h_std_out, coord);
		ch = _getch();
	}
	coord.X = 0;
	coord.Y = cnt + 1 + y;
	SetConsoleCursorPosition(h_std_out, coord);
	historyInsert(history_root, search_string);
	return search_string;
}