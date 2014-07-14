#include "Search.h"


UserFace::~UserFace()
{
	help_string.clear();
	input_words.clear();
}

void UserFace::print_usage()
{
	cout << "please input a word to lookup: " << endl;
}


vector<string> UserFace::GetInputWords()
{
	if (ERR == GetInputLine(cin))
		cout << "get input error: please input words."<< endl;

	if (!input_line.empty()) {
		input_words.clear();
		input_words = split(input_line);	
	}

	return input_words;	
}

int UserFace::GetInputLine(istream &in)
{
	if (in) {
		input_line.clear();
		in >> input_line;
		return OK;
	}
	
	return ERR;
}
