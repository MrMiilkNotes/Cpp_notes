#include<iostream>
#include<string>
#include<vector>

using namespace std;
int length;
int rest_len;
string out, tmp_rec;

void anagrams_by_stack_search(string& a, string& b, int idx);

void anagrams_by_stack()
{
	string a, b;

	while (cin >> out >> b) {
		a.assign(out.rbegin(), out.rend());
		out.clear();
		rest_len = length = a.size();
		cout << '[' << endl;
		if (length == b.size()) {
			anagrams_by_stack_search(a, b, 0);
		}
		cout << ']' << endl;
	}
}

void anagrams_by_stack_search(string& a, string& b, int idx)
{
	if (rest_len == 0) {
		cout << out << endl;
		return;
	}
	if (a.size()) {
		char tmp = a.back();
		tmp_rec.push_back(tmp);
		a.pop_back();
		out.push_back('i');
		out.push_back(' ');
		anagrams_by_stack_search(a, b, idx);
		out.pop_back();
		out.pop_back();
		a.push_back(tmp);
		tmp_rec.pop_back();
	}
	if (tmp_rec.size() && tmp_rec.back() == b[idx]) {
		char tmp_ = tmp_rec.back();
		tmp_rec.pop_back();
		out.push_back('o');
		out.push_back(' ');
		--rest_len;
		anagrams_by_stack_search(a, b, ++idx);
		++rest_len;
		out.pop_back();
		out.pop_back();
		tmp_rec.push_back(tmp_);
	}
}