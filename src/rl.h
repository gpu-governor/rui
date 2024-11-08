#include "raylib.h"
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RUI_COLOR;

const RUI_COLOR COLOR_BLACK = {0, 0, 0, 255};
const RUI_COLOR COLOR_RED = {255, 0, 0, 255};
const RUI_COLOR COLOR_GREEN = {0, 255, 0, 255};
const RUI_COLOR COLOR_WHITE = {255, 255, 255, 255};
const RUI_COLOR COLOR_BLUE = { 0, 121, 241, 255 };      // Raylib BLUE
const RUI_COLOR COLOR_SKYBLUE = { 102, 191, 255, 255 }; // Raylib SKYBLUE
const RUI_COLOR COLOR_DARKBLUE = { 0, 82, 172, 255 };   // Raylib DARKBLUE
const RUI_COLOR COLOR_DARKGRAY = {80, 80, 80, 255};
const RUI_COLOR COLOR_LIGHTGRAY = { 200, 200, 200, 255 };




// Function to convert RUI_COLOR to raylib Color
Color toRaylibColor(RUI_COLOR color) {
    Color raylibColor;
    raylibColor.r = color.r;
    raylibColor.g = color.g;
    raylibColor.b = color.b;
    raylibColor.a = color.a;
    return raylibColor;
}


void draw_rectangle_from_rect(Rectangle rect, RUI_COLOR color){
	Color raylibColor = toRaylibColor(color);
    DrawRectangleRec(rect, raylibColor); 
}
void draw_rectangle_lines(int x, int y, int width, int height, RUI_COLOR color) {
    Color raylibColor = toRaylibColor(color);
    DrawRectangleLines(x, y, width, height, raylibColor);
}

void draw_text(char * text,int  x,int  y, int font_size, RUI_COLOR color){
	Color raylibColor = toRaylibColor(color);  // Convert color
	DrawText(text, x, y, font_size, raylibColor); 
}
// Function to draw a rectangle with text and custom color
void draw_rectangle(int x, int y, int w, int h, RUI_COLOR color) {
    Color raylibColor = toRaylibColor(color);  // Convert color
    DrawRectangle(x, y, w, h, raylibColor);  // Draw rectangle
}
void draw_rectangle_rounded(Rectangle rect,float roundness , int seg, RUI_COLOR color) {
	Color raylibColor = toRaylibColor(color);
    DrawRectangleRounded(rect, 0.2f, 10, raylibColor);
}

// Function to initialize the window
void init(char * title, int w, int h) {
    InitWindow(w, h, title);  // Corrected to match raylib InitWindow signature
    SetTargetFPS(60);  // Set FPS limit
}

// Function to close the window
void quit() {
    CloseWindow();  // Clean up and close the window
}
