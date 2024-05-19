#include "Widgets.h"

#include <utility>

using namespace std;

Menu::Menu(const button& but1, const button& but2, const button& but3, const background& back): english(but1), french(but2), exit(but3), wallpaper(back), state("menu"){}

void Menu::Draw(){
    UpdateMusicStream(wallpaper.music);

    // Menu page
    BeginDrawing();

    // background
    wallpaper.Draw();

    // buttons
    french.Draw({200, 320});
    DrawText(TextFormat("French"), 275, 340, 40, BLUE);
    english.Draw({200, 415});
    DrawText(TextFormat("English"), 275, 435, 40, BLUE);
    exit.Draw({200, 510});
    DrawText(TextFormat("Exit"), 305, 535, 40, BLUE);


    // function of each button
    if (exit.action) state = "exit";
    if (english.action) state = "english", wallpaper.initialize();
    if (french.action) state = "french", wallpaper.initialize();

    EndDrawing();

}

dic_window::dic_window(const button& but3, const button& but2, textBox box1, const background& back): returns(but3), sound(but2), curser(-1), action_sound(true), timer((float)0), target("\0"), letter_count(0), description("\0"), box(box1), page(back), target_locked(false), increase(false){
    language.build("english.txt", "french.txt");
    eng.build(language.english);
    fr.build(language.french);
}

void dic_window::build(){
    eng.build(language.english);
    fr.build(language.french);
}

void dic_window::refresh(){
    for(auto& it:target) it = '\0';
    for(auto& it:description) it = '\0';
    letter_count = 0;
    target_locked = false;
    increase  =false;
    arr.clear();

}
void dic_window::search_bar(string& msg, int x, int y, int width, int height, int font){
    int key = GetCharPressed();

    if (!increase) {
        // Check if more characters have been pressed on the same frame
        while (key > 0) {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letter_count < 3005)) {
                target[letter_count] = (char) key;
                target[letter_count + 1] = '\0';
                letter_count++;
            }
            else break;
            key = GetCharPressed();
        }
        if (IsKeyDown(KEY_BACKSPACE)) {
            target_locked = false;
            timer += GetFrameTime();
            if (timer >= 0.1f) {
                if (letter_count) letter_count--;
                target[letter_count] = '\0';
                timer = 0;
            }
        }
        if (IsKeyPressed(KEY_ENTER)){
            letter_count = 0;
            if (arr.empty()) increase = true, curser = 0;
            else{
                for(auto it:arr[curser]){
                    target[letter_count] = it;
                    letter_count++;
                }
                target[letter_count] = '\0';
            }
            target_locked = true;
            returns.sound();
        }
    }
    else{
        // Check if more characters have been pressed on the same frame
        while (key > 0) {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letter_count < 3005)) {
                description[letter_count] = (char) key;
                description[letter_count + 1] = '\0';
                letter_count++;
            }
            key = GetCharPressed();
        }
        if (IsKeyDown(KEY_BACKSPACE)) {
            timer += GetFrameTime();
            if (timer >= 0.1f) {
                if (letter_count) letter_count--;
                description[letter_count] = '\0';
                timer = 0;
            }
        }
        if (IsKeyPressed(KEY_ENTER)){
            if (msg == "english") eng.dictionary[(string)target] = (string)description;
            else fr.dictionary[(string)target] = (string)description;
            eng.close();
            fr.close();
            build();
            refresh();
        }

    }
    // Draw the text box outline
    DrawRectangle(x, y, width, height, WHITE);
    DrawRectangleLines(x, y, width, height, BLACK);
    DrawText(target, x + 5, y + 5, font, BLACK);

}

void dic_window::Draw(string& msg){



    BeginDrawing();

    page.Draw();

    // buttons
    returns.Draw({280, 660});
    DrawText(TextFormat("Return"), 300, 675, 35, BLUE);
    sound.Draw({600, 100});
    if (sound.action) action_sound ^= true;
    if (action_sound) UpdateMusicStream(page.music);

    search_bar(msg, 190, 150, 350, 40, 30);



    if (returns.action){
        msg = "menu";
        page.initialize();
        refresh();
    }
    if (!target_locked){
        if (msg == "english") eng.search_pattern((string)target, arr);
        else fr.search_pattern((string)target, arr);
        box.Draw_candidate(arr, curser);
    }
    else if (increase){
        string ans = (string)description;
        box.Draw(ans);
    }
    else{
        if (msg == "english") box.Draw(eng.dictionary[target]);
        else box.Draw(fr.dictionary[target]);
    }

    EndDrawing();


}

textBox::textBox(Rectangle shape, Font font1, int fontsize, int space, Color color1): Box(shape), font(font1), old(), timer((float)0), beg(0), endo(0), curser(0),
                                                                                      font_size(fontsize), spacing(space), color(color1), pos({shape.x + 5, shape.y + 5})
                                                                                      {lineHeight = font.baseSize + spacing;}

void textBox::Draw(string& text){
    pos = {Box.x + 5, Box.y + 5};

    // Draw the text box outline
    DrawRectangle((int)Box.x, (int)Box.y, (int)Box.width, (int)Box.height, WHITE);
    DrawRectangleLines((int)Box.x, (int)Box.y, (int)Box.width, (int)Box.height, color);

    // Create a copy of the text to modify
    char textCopy[3005];
    for(int i = 0; i < text.size(); i++) textCopy[i] = text[i];
    textCopy[text.size()] = '\0';

    int totalLines = 0; // Count the total lines drawn

    // Start splitting and drawing the text
    char *line = strtok(textCopy, " ");
    char currentLine[3005] = ""; // Buffer for the current line
    int lineIndex = 0;

    while (line != nullptr){
        // Append the current word to the current line
        int wordLength = (int)strlen(line);
        // Check if the current line will exceed the width of the text box if the new word is added
        if (lineIndex > 0 && MeasureTextEx(font, currentLine, (float)font_size, (float)spacing).x + MeasureTextEx(font, line, (float)font_size, (float)spacing).x - 240 > Box.width){
            // Draw the current line if adding the new word would exceed the width
            DrawText(currentLine, (int)pos.x, (int)pos.y, font_size, color);
            pos.y += (float)(font.baseSize + spacing + 10); // Move to the next line
            currentLine[0] = '\0'; // Clear the line buffer
            lineIndex = 0; // Reset the line index
        }

        // Add the word to the current line
        strcat(currentLine, line);
        lineIndex += wordLength;

        // Move to the next word
        line = strtok(nullptr, " ");

        // Add a space between words if needed
        if (line != nullptr){
            strcat(currentLine, " ");
            lineIndex++;
        }
    }

    // Draw the last line if it wasn't drawn yet
    if (currentLine[0] != '\0') DrawText(currentLine, (int)pos.x, (int)pos.y, font_size, color);

}

void textBox::Draw_candidate(const vector<string>& arr, int& var){
    int key = GetCharPressed();
    endo = (int)arr.size();
    pos = {Box.x + 5, Box.y + 5};
    int op = (int)(Box.height / 13);
    if (!arr.empty() && old != arr[0]) var = 0, old = arr[0], curser = 0;
    else if (arr.empty()) var = -1;
    // Draw the text box outline
    DrawRectangle((int)Box.x, (int)Box.y, (int)Box.width, (int)Box.height, WHITE);
    DrawRectangleLines((int)Box.x, (int)Box.y, (int)Box.width, (int)Box.height, color);
    if (var >= 0) DrawRectangle((int)Box.x, (int)Box.y + op * curser, (int)Box.width, op + 10, BLUE);
    int tot = 0;
    for(int j = beg; j < endo; j++){
        // Create a copy of the text to modify
        char textCopy[3005];
        for(int i = 0; i < (int)arr[j].size(); i++) textCopy[i] = arr[j][i];
        textCopy[arr[j].size()] = '\0';

        DrawText(textCopy, (int)pos.x, (int)pos.y, font_size, color);
        pos.y += (float)(font.baseSize + spacing + 10); // Move to the next line
        tot++;
        if (tot == 13) break;
    }
    if (var >= 0) {
        if (IsKeyDown(KEY_DOWN)) {
            timer += GetFrameTime();
            if (timer >= 0.1f) {
                var++;
                curser = min(12, curser + 1);
                if (var == (int) arr.size()) {
                    var = 0;
                    beg = 0;
                    curser = 0;
                } else if (var >= 13) beg++;
                timer = 0;
            }
        }
        if (IsKeyDown(KEY_UP)) {
            timer += GetFrameTime();
            if (timer >= 0.1f) {
                var--;
                curser = max(curser - 1, 0);
                if (var < beg && var >= 0) beg--;
                else if (var == -1) var += endo, beg = max(0, endo - 13), curser = min(endo - 1, 12);
                timer = 0;
            }
        }
    }

}
