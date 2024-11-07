#include "raylib.h"

// Function to convert RUI_COLOR to raylib Color
Color toRaylibColor(RUI_COLOR color) {
    Color raylibColor;
    raylibColor.r = color.r;
    raylibColor.g = color.g;
    raylibColor.b = color.b;
    raylibColor.a = color.a;
    return raylibColor;
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
