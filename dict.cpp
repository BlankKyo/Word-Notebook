#include "dict.h"

dict::dict(const char *file): filename(file){
    ifstream fp(file);
    string line, key, def;
    int n, index;
    while(getline(fp, line)){
        n = (int)line.size();
        for(int i = 0; i < n; i++){
            if (line[i + 1] != ':') key += line[i];
            else{
                index = i + 3;
                break;
            }
        }
        for(int i = index; i < n; i++) def += line[i];
        dictionary[key] = def;
        key.clear();
        def.clear();
    }
    index = 0;
    fp.close();
    for(const auto& it:dictionary){
        rs += it.first;
        reel_index[index] = (int)rs.size() - index;
        index = (int)rs.size();
    }
    Suffix_array_sort(rs, pos, rank);
}

dict::dict(): filename(""){}

void dict::build(const char *file){
    filename = file;
    ifstream fp(file);
    string line, key, def;
    int n, index;
    while(getline(fp, line)){
        n = (int)line.size();
        for(int i = 0; i < n; i++){
            if (line[i + 1] != ':') key += line[i];
            else{
                index = i + 3;
                break;
            }
        }
        for(int i = index; i < n; i++) def += line[i];
        dictionary[key] = def;
        key.clear();
        def.clear();
    }
    index = 0;
    fp.close();
    for(const auto& it:dictionary){
        rs += it.first;
        reel_index[index] = (int)rs.size() - index;
        index = (int)rs.size();
    }
    Suffix_array_sort(rs, pos, rank);
}

void dict::search_pattern(const string& pat, vector<string>& Arr){
    Arr.clear();
    if (!pat.empty()) {
        pair<int, int> tot = count_sub(pos, rs, pat);
        string ans;
        for (int i = tot.first; i <= tot.second; i++) {
            if (reel_index[pos[i]]) {
                for (int j = 0; j < reel_index[pos[i]]; j++) ans += rs[pos[i] + j];
                Arr.emplace_back(ans);
                ans.clear();
            }
        }
    }
}
void dict::close(){
    ofstream fp(filename);
    for(const auto& it:dictionary){
        if (!it.second.empty()) fp << it.first << " : " << it.second << '\n';
    }
    fp.close();
    cout << filename << '\n';
    rs.clear();
    pos.clear();
    rank.clear();
    reel_index.clear();
    dictionary.clear();
}

item::item(): english(""), french(""){}

item::item(const char* lang1, const char* lang2): english(lang1), french(lang2){}

void item::build(const char* lang1, const char* lang2){
    english = lang1;
    french = lang2;
}
