#ifndef LIBRARY_ALGO_STRING_ALGO_H
#define LIBRARY_ALGO_STRING_ALGO_H

#include <bits/stdc++.h>

using namespace std;

// concerns one query search in O(m + n)
void KMP_search(const string& txt, string pattern, vector<int>& index);
void computeLPSArray(const string& pattern, int pattern_size, int* lps);

// concerns suffix array
void GetHeight(string txt, vector<int>& Pos, vector<int>& Rank, vector<int>& lcp);
void computeLcpArray(string pattern, int pattern_size, int* Lcp);
void Suffix_array_sort(string msg, vector<int>& Pos, vector<int>& Rank);
bool find_sub(const vector<int>& Pos, const string& msg, const string& ref);
pair<int,int> count_sub(const vector<int>& Pos, const string& msg, const string& ref);


#endif //LIBRARY_ALGO_STRING_ALGO_H
