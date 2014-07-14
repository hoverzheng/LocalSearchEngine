#include "Search.h"

Dict::Dict(string &key, string &fname, unsigned long pos)
{
	DictInfo di;

	next = NULL;
	name = key;

	di.fname = fname;
	di.pos = pos;
	di_vec.push_back(di);	
	npos = 1;
}

string Dict::GetName() const
{
	return name;	
}


Dict* Dict::Next() const
{
	return next;
}

void Dict::SetNext(Dict *n) 
{
	next = n;
}


/*
bool Dict::operator==(const Dict &d2) const
{
	return (name == d2.GetName());
}
*/


int Dict::AddDictInfo(string &fname, unsigned long pos)
{
	DictInfo di;	

	di.fname = fname;
	di.pos = pos;
	di_vec.push_back(di);
	npos++;

	return OK;
}


void Dict::PrintDictInfo() const
{
	vector<DictInfo>::const_iterator it;

	cout << "find word:  <" << name << "> and total position: " << npos << endl;

	for (it=di_vec.begin(); it<di_vec.end(); it++)
		cout << "file: " << it->fname << ", line: " << it->pos << endl;
}
