#include "Search.h"


/******************************* 
 *
 * Tools class 
 *
 *******************************/
vector<string>  Tools::split(const string &s) const
{
    vector<string> ret;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()) {
        while (i != s.size()  && isspace(s[i])) //jump space
            ++i;

        string_size j = i;
        while (j != s.size() && !isspace(s[j])) {
            ++j;
        }   

        if (i != j) {
            ret.push_back(s.substr(i, j - i));
            i = j;
        }   
    }   
    return ret;
}


/******************************* 
 *
 * SearchSearver class 
 *
 *******************************/
SearchServer::~SearchServer()
{
	if (!flist.empty())	
		flist.clear();	
}

SearchServer::SearchServer(const char* root)
{
	nfiles = 0;
	nwords = 0;	
	root_dir = root;
}

// get all file list in dir @dir
int SearchServer::get_file_list(const char *dir)
{
    struct stat st;
    DIR *pdir;
    struct dirent *dp;
    char dpath[MAXNAMLEN];
	const char *psdir = NULL;

    if (NULL == dir)
        return ERR;
	
	psdir = dir;
    if (0 != stat(dir, &st)) {
		cout << "open" << dir << "error" << endl;
        return ERR;
    }   
    
    if (is_dir(st)) {
        pdir = opendir(psdir);
        if (NULL == pdir) {
            return ERR;
        }

        while ((dp = readdir(pdir)) != NULL) {
            if (!strcmp(dp->d_name,".") || !strcmp(dp->d_name, ".."))
                continue;
            snprintf(dpath, MAXNAMLEN, "%s/%s", dir, dp->d_name);
            if (ERR == get_file_list(dpath))
                continue;
        }
        closedir(pdir);
    } else {
		// save file list to flist vector.	
		flist.push_back(string(dir));
		nfiles += 1;
    }
	return OK;
}


unsigned long SearchServer::get_words_count() const
{
	return db.get_total_count();
}

// get file list size
unsigned int SearchServer::get_flist_size() const
{
	return nfiles;	
}


int SearchServer::load_words(string &fname)
{
    string line;
    vector<string>::iterator it;
	unsigned long nline = 0;
    ifstream ins(fname.c_str()); 

    while (ins.good()) {
        getline(ins, line);
		nline++;
		
        vector<string> v = split(line);
        for (it = v.begin(); it < v.end(); it++) {
			db.insert_dict(*it, fname, nline);	
		}
    }

    ins.close();
	return OK;	
}


int SearchServer::init()
{
	get_file_list(root_dir);
	if (flist.empty()) {
		cout << "server init failed..." << endl;
		return ERR;	
	}

	// get word and save word to Storage from every file.
	vector<string>::iterator it;
	for (it = flist.begin(); it < flist.end(); it++) {
		load_words(*it);
		cout << "load words from " << *it << "  successfully!" << endl;
	}

	return OK;
}


// start search server
int SearchServer::start()
{
	cout << "Search Server start ..." << endl;	

	init();
	cout << "total file count :" << get_flist_size() << endl;
	cout << "total words count :" << get_words_count() << endl;
	
	while (1) {
		// get input 
		vector<string> vstr;
		face.print_usage();
		vstr = face.GetInputWords();
		
		vector<string>::iterator it;
		for (it=vstr.begin(); it < vstr.end(); it++) {
			Dict *p = NULL;	
			p = db.find_dict(*it);
			if (p) p->PrintDictInfo();
		}
	}
	
	return OK;
}



/********************************
 *
 * Storage class
 *
 ********************************/
StorageDict::StorageDict()
{
	total_count = 0;
	for (int i = 0; i < NHASH; i++)
		buck[i] = 0;
}

unsigned int StorageDict::hash(string name) 
{
	unsigned int h = 0;
	const char *p = name.c_str();
	
	for ( ; *p; p++)	
		h = MULT * h + *p;
	return h % NHASH;
}


// insert one dict to dict hash table
int StorageDict::insert_dict(string &key, string &fname, unsigned long pos)
{
	unsigned int h;
	Dict *pd = NULL;
	
	h = hash(key);
	for (pd = buck[h]; pd != NULL; pd = pd->Next()) {
		if (pd->GetName() == key) { // find one
			pd->AddDictInfo(fname, pos);
			total_count++;
			return OK;
		}
	}
	
	// I don't find the same key word, create new one and insert it.
	Dict *d = new Dict(key, fname, pos);	
	d->SetNext(buck[h]);
	buck[h] = d;
	total_count++;

	return OK;
}


// find one dict in hash table
Dict* StorageDict::find_dict(string key)
{
	unsigned int h;
	Dict *pd = NULL;
	
	h = hash(key);
	for (pd = buck[h]; pd != NULL; pd = pd->Next())
		if (pd->GetName() == key)
			return pd;

	return NULL;
}


unsigned long StorageDict::get_total_count() const
{
	return total_count;
}
