#include "Widgets.h"
#include "raylib.h"

using namespace std;

int screen_width = 700;
int screen_height = 800;
int fps = 60;
bool running = true;
string current_state;

int main() {

    const char *longText = "This is a long string that needs to be wrapped into multiple lines to fit within the specified area. Raylib provides functions to measure text width, allowing you to split the text as needed and draw each line separately.";
    // initialize Window and AudioDevice
    InitWindow(screen_width, screen_height, "Dictionary");
    SetTargetFPS(fps);
    Font font = LoadFontEx("Font/monogram.ttf", 64, nullptr, 0);
    InitAudioDevice();

    // load images
    Texture2D image1 = LoadTexture("Image/Game_background.png");
    Texture2D image2 = LoadTexture("Image/big_brain.png");
    Image photo1 = LoadImage("Image/button1.png");
    Image photo2 = LoadImage("Image/button2.png");
    Image photo3 = LoadImage("Image/button3.png");
    Image photo4 = LoadImage("Image/sound_button.png");
    Image photo5 = LoadImage("Image/dic_button2.png");
    Image photo6 = LoadImage("Image/dic_button3.png");

    // create backgrounds
    background back(image1, 0, -1, 2, 2, 0, 0,"Sounds/kei theme Kung fu panda.mp3");
    background backend(image2, 0, 0, 1.3, 1.9, 350, 405,"Sounds/luffy_gear5.mp3");

    // create buttons
    button but1(photo1, 0.65, 0.65);
    button but2(photo2, 0.65, 0.65);
    button but3(photo3, 0.65, 0.65);
    button but4(photo4, 0.2, 0.2);
    button but5(photo5, 0.6, 0.6);
    button but6(photo6, 0.6, 0.6);

    // initialize windows
    Menu display(but1, but2, but3, back);
    textBox box_text({ 50, 200, 600, 400 }, GetFontDefault(), 30, 10, BLACK);
    dic_window dictionary(but6, but4,box_text, backend);


    while(running){
        current_state = display.state;
        if (current_state == "exit"){
            running = false;
            CloseAudioDevice();
            CloseWindow();
        }
        else if (current_state == "menu") display.Draw();
        else dictionary.Draw(display.state);

    }

}






