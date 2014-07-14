#ifndef __SEARCH_H_
#define __SEARCH_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define OK 0
#define ERR -1

using namespace std;

class Tools {
public: 
	vector<string> split(const string &s) const;
};

// 用户接口
class UserFace : Tools {
public:
	UserFace():help_string(""),input_line(""),input_words(0) {}
	~UserFace();

	void print_usage();
	vector<string> GetInputWords();
private:
	int GetInputLine(istream &i);

	string help_string;
	string input_line;
	vector<string> input_words;
};


// 每个单词对应的信息，可能对应多个DictInfo结构。
typedef struct {
	string fname;
	unsigned long pos;		
}DictInfo;

class Dict { 	// 单词类
public:
	Dict():next(0),name(""),npos(0){}
	Dict(string &key, string &fname, unsigned long pos);
	~Dict();

	int AddDictInfo(string &fname, unsigned long pos);	
	void PrintDictInfo() const;
	//bool operator ==(const Dict &d2) const;
	
	string GetName() const;
	Dict *Next() const;
	void SetNext(Dict *n);

private:
	Dict *next;
	string name;
	vector<DictInfo> di_vec;
	int npos;
};


// storage hash table to save dict
class StorageDict {
public:
	StorageDict();
	~StorageDict() {}
	int insert_dict(string &key, string &fname, unsigned long pos);
	Dict *find_dict(string name) ;
	unsigned long get_total_count() const;

private:
	static const int NHASH = 29989;  // from <<programmingpearls>>
	static const int MULT = 31;		 // from <<programmingpearls>>
	unsigned long total_count;
	Dict *buck[NHASH];
	unsigned int hash(string name);	
};


// location search engine
class SearchServer : Tools {
public:
	SearchServer():nfiles(0),nwords(0), root_dir("/") {}
	SearchServer(const char* root);
	~SearchServer();

	int init();
	int start();
	void CreateOutput(string &result);
	unsigned int get_flist_size() const;
	unsigned long get_words_count() const;
	//vector<string> split(const string &s) const;
	//StorageDict db;
private:
	
	int get_file_list(const char *r);  //get all file list from dir:root_dir
	int load_words(string &fname);

	inline bool is_dir(struct stat st) 
	{
		return (S_ISDIR(st.st_mode) || S_ISLNK(st.st_mode));
	}
	
	vector<string> flist; // file list 
	unsigned int nfiles;  // total files in @root_dir
	unsigned long nwords; // total words in all files 
	const char *root_dir; // root dir
	UserFace face;
	StorageDict db;
};


#endif
