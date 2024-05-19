#ifndef DICTIONARY_WIDGETS_H
#define DICTIONARY_WIDGETS_H
#include "raylib.h"
#include "Graphics/Background.h"
#include "Graphics/Buttons.h"
#include "dict.h"

using namespace std;

class Menu{
public:
    button english;
    button french;
    button exit;
    string state;
    background wallpaper;
public:
    Menu(const button& but1, const button& but2, const button& but3, const background& back);
    void Draw();
};

class textBox{
public:
    Rectangle Box;
    Font font;
    int font_size;
    int beg;
    int endo;
    int curser;
    int spacing;
    float timer;
    string old;
    Color color;
    Vector2 pos;
    int lineHeight;
public:
    textBox(Rectangle shape, Font font1, int fontsize, int space, Color color1);
    void Draw(string& text);
    void Draw_candidate(const vector<string>& arr, int& var);
};

class dic_window{
public:
    background page;
    textBox box;
    item language;
    dict eng;
    dict fr;
    button returns;
    button sound;
    vector<string> arr;
    int curser;
    float timer;
    char target[3005];
    char description[3005];
    bool target_locked;
    bool increase;
    bool action_sound;
    int letter_count;
public:
    dic_window(const button& but3, const button& but2, textBox box1, const background& back);
    void build();
    void refresh();
    void search_bar(string& msg, int x, int y, int width, int height, int font);
    void Draw(string& msg);
};



#endif //DICTIONARY_WIDGETS_H
