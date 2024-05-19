#ifndef DICTIONARY_DICT_H
#define DICTIONARY_DICT_H

#include <bits/stdc++.h>
#include "string_algo.h"
#include <fstream>

using namespace std;

class dict{
public:
    const char* filename;
    map<string, string> dictionary;
    string rs;
    map<int,int> reel_index;
    vector<int> pos;
    vector<int> rank;
public:
    dict();
    explicit dict(const char *file);
    void search_pattern(const string& pat, vector<string>& Arr);
    void build(const char *file);
    void close();
};

class item{
public:
    const char* english;
    const char* french;
public:
    item();
    explicit item(const char* lang1, const char* lang2);
    void build(const char* lang1, const char* lang2);
};

#endif //DICTIONARY_DICT_H
