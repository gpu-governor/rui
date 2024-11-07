#include<stdint.h>
#include<stdbool.h>
#include"raylib.h"

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


typedef struct Style {
    // Colors for various UI elements
    RUI_COLOR background_color;
    RUI_COLOR text_color;
    RUI_COLOR button_color;
    RUI_COLOR button_hover_color;
    RUI_COLOR button_active_color;
    RUI_COLOR border_color;
    RUI_COLOR frame_color;
    // Text properties
    float fontSize;
}RUI_STYLE;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} rui_rect;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    char *text;
    int font_size;
    RUI_COLOR button_color;
    RUI_COLOR text_color;
    rui_rect button_bounds;  // Updated to use rui_rect
    RUI_COLOR hover_color;
    RUI_COLOR clicked_color;
    bool is_hovered;
    bool is_clicked;
} rui;

// Convert rui_rect to Raylib Rectangle
Rectangle toRaylibRectangle(rui_rect rect) {
    Rectangle raylibRect = { rect.x, rect.y, rect.width, rect.height };
    return raylibRect;
}

//================================ GENERAL ====================================
void init_ui(char * title, int w, int h){
	init(title,w,h);
}
void quit_ui(){
	quit();
}
// RUI_STYLE create_style(){
// 	RUI_STYLE DarkTheme;
// 	DarkTheme.background_color = COLOR_BLACK;
// 	DarkTheme.background_color = BLACK;
// 	
// } 
// void style_ui(RUI_STYLE *style){
// 	
// }
//========================================= BUTTONS ==================================
rui button_ui(char * text, float x, float y) {
    rui new_button = {0};
	new_button.x = (float)x;
	new_button.y = (float)y;
	new_button.width = (float)70;
	new_button.height = (float)30;
	new_button.font_size=10;
    new_button.button_bounds = (rui_rect){ new_button.x, new_button.y, new_button.width, new_button.height }; // TODO : this is raylib specific function, shouldn't be here
    new_button.button_color = COLOR_BLUE;
    new_button.hover_color = COLOR_SKYBLUE;
    new_button.clicked_color = COLOR_DARKBLUE;
    new_button.text_color = COLOR_WHITE;
    new_button.is_hovered = false;
    new_button.is_clicked = false;
    return new_button;
}

void render_button(rui *button) {
    RUI_COLOR draw_color = button->button_color;  // Updated to match `button_color`

    if (button->is_hovered) draw_color = button->hover_color;
    if (button->is_clicked) draw_color = button->clicked_color;

    float roundness = 0.2f;
    int segments = 10;

    // Convert rui_rect to Raylib Rectangle
    Rectangle raylibRect = toRaylibRectangle(button->button_bounds);

    // Use the Raylib Rectangle for drawing
    draw_rectangle_rounded(raylibRect, roundness, segments, draw_color);
    
    draw_text(button->text, button->button_bounds.x + 10, button->button_bounds.y + 10, button->font_size, button->text_color);
}

bool update_button(rui *button) {
    Vector2 mousePoint = GetMousePosition();

    // Convert rui_rect to Raylib Rectangle
    Rectangle raylibRect = toRaylibRectangle(button->button_bounds);

    button->is_hovered = CheckCollisionPointRec(mousePoint, raylibRect);
    button->is_clicked = false;

    if (button->is_hovered) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            button->is_clicked = true;
        }
        else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            return true; // Button was clicked
        }
    }
    return false; // Button was not clicked
}
