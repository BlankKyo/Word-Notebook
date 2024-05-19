#include "string_algo.h"

void computeLPSArray(const string& pattern, int pattern_size, int* lps){
    int len = 0;
    for(int i = 0; i < pattern_size; i++) lps[i] = 0;
    int i = 1;
    while (i < pattern_size) {
        if (pattern[i] == pattern[len]) len++, lps[i] = len, i++;
        else {
            if (len) len = lps[len - 1];
            else i++;
        }
    }
}

void GetHeight(string txt, vector<int>& Pos, vector<int>& Rank, vector<int>& lcp){
    txt += '$';
    int n = (int)Pos.size();
    int h = 0, k, j;
    for(int i = 0; i < (n - 1); i++){
        k = Pos[Rank[i] - 1];
        while(txt[i + h] == txt[k + h]) h++;
        lcp[Rank[i] - 1] = h;
        if (h) h--;
    }
}
void computeLcpArray(string pattern, int pattern_size, int* Lcp){
    reverse(pattern.begin(), pattern.end());
    int len = 0;
    for(int i = 0; i < pattern_size; i++) Lcp[i] = 0;
    int i = 1;
    while (i < pattern_size) {
        if (pattern[i] == pattern[len]) len++, Lcp[i] = len, i++;
        else {
            if (len) len = Lcp[len - 1];
            else i++;
        }
    }
    reverse(Lcp, Lcp + pattern_size);
}

// search function that points all the occurrences of pattern in txt in O(m + n) complexity.
void KMP_search(const string& txt, string pattern, vector<int>& index){
    int txt_size = (int) txt.size();
    int pattern_size = (int) pattern.size();
    int lps[pattern_size];
    computeLPSArray(pattern, pattern_size, lps);
    int i = 0;
    int j = 0;
    while ((txt_size - i) >= (pattern_size - j)) {
        if (pattern[j] == txt[i]) i++, j++;
        if (j == pattern_size) index.emplace_back(i - j), j = lps[j - 1];
        else if (i < txt_size && pattern[j] != txt[i]) {
            if (j) j = lps[j - 1];
            else i++;
        }
    }
}

// give array of the index of the suffixes of the string msg sorted
void Suffix_array_sort(string msg, vector<int>& Pos, vector<int>& Rank){
    int n = (int) msg.size() + 1;
    msg += '$';
    int k = (int)log2(n), op = 1 << k;
    if (n > op) k++;
    vector<int> classe(n);
    vector<pair<char,int>> array(n);
    for(int i = 0; i < n; i++) array[i].first = msg[i], array[i].second = i;
    sort(array.begin(), array.end());
    int ir = 0;
    classe[array[0].second] = 0;
    for(int i = 1; i < n; i++){
        if (array[i].first != array[i-1].first) ir++;
        classe[array[i].second] = ir;
    }
    vector<pair<pair<int,int>,int>> shuffle(n), copy(n);
    int it = 1, opp = 1;
    while(it <= k){
        for(int i = 0; i < n; i++){
            shuffle[i].first.first = classe[i];
            shuffle[i].first.second = classe[(i + opp) % n];
            shuffle[i].second = i;
        }

        // radix_sort O(n)
        vector<int> counter(ir + 2,0), counter1(ir + 2,0);
        for(auto itt:shuffle) counter1[itt.first.second + 1]++;
        for(int i = 1; i <= ir; i++) counter1[i] += counter1[i - 1];
        for(int i = 0; i < n; i++){
            copy[counter1[shuffle[i].first.second]] = shuffle[i];
            counter1[shuffle[i].first.second]++;
        }
        for(auto itt:copy) counter[itt.first.first + 1]++;
        for(int i = 1; i <= ir; i++) counter[i] += counter[i - 1];
        for(int i = 0; i < n; i++){
            shuffle[counter[copy[i].first.first]] = copy[i];
            counter[copy[i].first.first]++;
        }

        // update classe
        ir = 0;
        classe[shuffle[0].second] = 0;
        for(int i = 1; i < n; i++){
            if (shuffle[i].first.first != shuffle[i-1].first.first || shuffle[i].first.second != shuffle[i-1].first.second) ir++;
            classe[shuffle[i].second] = ir;
        }
        opp *= 2;
        it++;
    }
    vector<pair<int,int>> yo(n);
    Rank.resize(n);
    for(int i = 0; i < n; i++){
        yo[i].first = classe[i];
        yo[i].second = i;
    }
    sort(yo.begin(), yo.end());
    for(auto itt:yo) Pos.emplace_back(itt.second), Rank[itt.second] = itt.first;
}

// find ref in msg
bool find_sub(const vector<int>& Pos, const string& msg, const string& ref){
    int n = (int) msg.size() + 1;
    int i = 0, j = n, mid, check;
    int m = (int) ref.size();
    bool ans = false;
    while(i <= j){
        check = 1;
        mid = (i + j) / 2;
        for(int ix = 0; ix < m; ix++){
            if (Pos[mid] + ix >= n || msg[Pos[mid] + ix] < ref[ix]){
                check = 0;
                i = mid + 1;
                break;
            }
            else if (msg[Pos[mid] + ix] > ref[ix]){
                check = 0;
                j = mid - 1;
                break;
            }
        }
        if (check) break;
    }
    if (i <= j) ans = true;
    return ans;
}

pair<int,int> count_sub(const vector<int>& Pos, const string& msg, const string& ref){
    int n = (int) msg.size();
    int i = 0, j = n, mid, check;
    int m = (int) ref.size();
    int maxo = -1, mino = 0;
    while(i <= j){
        check = 1;
        mid = (i + j) / 2;
        for(int ix = 0; ix < m; ix++){
            if (Pos[mid] + ix >= n || msg[Pos[mid] + ix] < ref[ix]){
                check = 0;
                i = mid + 1;
                break;
            }
            else if (msg[Pos[mid] + ix] > ref[ix]){
                check = 0;
                j = mid - 1;
                break;
            }
        }
        if (check) maxo = mid, i = mid + 1;
    }
    i = 0, j = n;
    while(i <= j){
        check = 1;
        mid = (i + j) / 2;
        for(int ix = 0; ix < m; ix++){
            if (Pos[mid] + ix >= n || msg[Pos[mid] + ix] < ref[ix]){
                check = 0;
                i = mid + 1;
                break;
            }
            else if (msg[Pos[mid] + ix] > ref[ix]){
                check = 0;
                j = mid - 1;
                break;
            }
        }
        if (check) mino = mid, j = mid - 1;
    }
    return make_pair(mino, maxo);
}




