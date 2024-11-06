#include<stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RUI_COLOR;

const RUI_COLOR COLOR_BLACK = {0, 0, 0, 255};
const RUI_COLOR COLOR_RED = {255, 0, 0, 255};
const RUI_COLOR COLOR_GREEN = {0, 255, 0, 255};
const RUI_COLOR COLOR_BLUE = {0, 0, 255, 255};
const RUI_COLOR COLOR_WHITE = {255, 255, 255, 255};

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
	int xpos;
	int ypos;
	char * text;
	RUI_COLOR button_color;
}rui;

//================================ GENERAL ====================================
void init_ui(char * title, int w, int h){
	init(title,w,h);
}
void quit_ui(){
	quit();
}
RUI_STYLE DarkTheme;
DarkTheme.background_color = 
void style_ui(RUI_STYLE *style){
	
}
//========================================= BUTTONS ==================================
rui button_ui(char * text, int x, int y){
	rui new_button;
	new_button.text = text;
	new_button.xpos=x;
	new_button.ypos=y;
	// default 
	new_button.button_color = DarkTheme.buttoncolor;
	return new_button;
}

void render_button(rui * new_button){
	int w = 150;
	int h = 50;
	int x = new_button->xpos;
	int y = new_button->ypos;
	char * text = new_button->text;
	RUI_COLOR color = COLOR_RED;
	RUI_COLOR text_color = COLOR_WHITE;

	draw_rectangle(x,y,w,h, color);
	draw_text(text, x + 10, y + 10, 20, text_color);
}
