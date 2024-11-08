#include<stdint.h>
#include<stdbool.h>
#include"raylib.h"
#include"rl.h"
#include <string.h>

#define MAX_LINES 10 // for text boxes
#define MAX_LINE_LENGTH 128 // for text boxes
#define MAX_TEXT_LENGTH 256 // for text entry




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

// Convert rui_rect to Raylib Rectangle
Rectangle toRaylibRectangle(rui_rect rect) {
    Rectangle raylibRect = { rect.x, rect.y, rect.width, rect.height };
    return raylibRect;
}

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

    // text box
    rui_rect textbox_bounds;                // Position and size of the text box
    char textbox_text[MAX_LINES][MAX_LINE_LENGTH]; // 2D array to store text lines
    int line_count;                  // Current line count in the box
    RUI_COLOR background_color;          // Background color of the text box
    bool active;                     // True when the text box is selected
    int current_line;                // Current line being edited
    int current_pos;                 // Current position in the current line
    const char *placeholder;         // Placeholder text
    RUI_COLOR placeholder_color;         // Placeholder text color

	// text entry
	rui_rect textentry_bounds;                // Position and size of the text box
    char textentry_text[MAX_TEXT_LENGTH];  // Text entered by the user
    int cursor_position;         // Current position of the cursor within the text
    int text_offset;             // Offset for scrolling text within bounds
} rui;


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
	new_button.text = text;
	new_button.x = (float)x;
	new_button.y = (float)y;
	new_button.width = (float)70;
	new_button.height = (float)30;
	new_button.font_size=10;
    new_button.button_bounds = (rui_rect){ new_button.x, new_button.y, new_button.width, new_button.height };
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
    // Measure text width to center it horizontally
        int text_width = MeasureText(button->text, button->font_size);
        int text_x = button->button_bounds.x + (button->button_bounds.width - text_width) / 2;
        int text_y = button->button_bounds.y + (button->button_bounds.height - button->font_size) / 2;
    
        draw_text(button->text, text_x, text_y, button->font_size, button->text_color);
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

// ================================ TEXT BOX ==================================
// Initialize a multiline text box with placeholder text
rui text_box_ui(float x, float y, float width, float height, int font_size, RUI_COLOR text_color, RUI_COLOR background_color, const char *placeholder) {
    rui text_box = {0};
    text_box.textbox_bounds = (rui_rect){ x,y,width,height };
    text_box.font_size = font_size;
    text_box.text_color = text_color;
    text_box.background_color = background_color;
    text_box.line_count = 1;  // Start with one line
    text_box.current_line = 0;
    text_box.current_pos = 0;
    text_box.active = false;
    text_box.placeholder = placeholder;
    text_box.placeholder_color = COLOR_LIGHTGRAY;
    memset(text_box.textbox_text, 0, sizeof(text_box.textbox_text)); // Initialize text array with empty strings
    return text_box;
}

// Draws the multiline text box and handles cursor blinking
void render_text_box(rui *text_box) {
	// Convert rui_rect to Raylib Rectangle
	Rectangle raylibRect = toRaylibRectangle(text_box->textbox_bounds);
    draw_rectangle_from_rect(raylibRect, text_box->background_color);// Draw background
    draw_rectangle_lines(text_box->textbox_bounds.x, text_box->textbox_bounds.y, text_box->textbox_bounds.width, text_box->textbox_bounds.height, COLOR_DARKGRAY);// Draw border

    bool is_empty = (text_box->line_count == 1 && strlen(text_box->textbox_text[0]) == 0);

    // If text box is empty and not active, show placeholder text
    if (is_empty && !text_box->active) {
            draw_text(text_box->placeholder, text_box->textbox_bounds.x + 5, text_box->textbox_bounds.y + 5, text_box->font_size, text_box->placeholder_color);
    } else {
        // Draw each line of text
        for (int i = 0; i < text_box->line_count; i++) {
            int y_offset = i * (text_box->font_size + 5); // Adjust line spacing
            draw_text(text_box->textbox_text[i], text_box->textbox_bounds.x + 5, text_box->textbox_bounds.y + y_offset + 5, text_box->font_size, text_box->text_color);
        }

        // Draw blinking cursor if active
        if (text_box->active && text_box->current_line < MAX_LINES) {
            int cursor_x = text_box->textbox_bounds.x + 5 + MeasureText(text_box->textbox_text[text_box->current_line], text_box->font_size);
            int cursor_y = text_box->textbox_bounds.y + (text_box->current_line * (text_box->font_size + 5)) + 5;
            if ((GetTime() * 2.0f) - (int)(GetTime() * 2.0f) < 0.5f) {  // Blinking effect
                // DrawRectangle(cursor_x, cursor_y, 2, text_box->font_size, text_box->text_color);
                draw_rectangle(cursor_x, cursor_y, 2, text_box->font_size, text_box->text_color);
            }
        }
    }
}

// Updates the multiline text box with user input and handles automatic line wrapping
void update_text_box(rui *text_box) {
    if (text_box->active) {
        int key = GetCharPressed();

        while (key > 0) {
            // Check for printable characters and prevent overflow
            if (key >= 32 && key <= 125) {
                int text_width = MeasureText(text_box->textbox_text[text_box->current_line], text_box->font_size);
                
                // Check if adding the character would exceed the box width
                if (text_width + MeasureText("A", text_box->font_size) >= text_box->textbox_bounds.width - 10 && text_box->current_line < MAX_LINES - 1) {
                    // Move to the next line if current line width exceeds bounds
                    text_box->current_line++;
                    text_box->line_count++;
                    text_box->current_pos = 0;
                }

                // Add the character to the current line if within bounds
                if (text_box->current_pos < MAX_LINE_LENGTH - 1) {
                    text_box->textbox_text[text_box->current_line][text_box->current_pos] = (char)key;
                    text_box->current_pos++;
                    text_box->textbox_text[text_box->current_line][text_box->current_pos] = '\0';  // Null-terminate the string
                }
            } else if (key == KEY_ENTER || key == '\n') { // Move to a new line on Enter key
                if (text_box->line_count < MAX_LINES) {
                    text_box->current_line++;
                    text_box->line_count++;
                    text_box->current_pos = 0;
                }
            }
            key = GetCharPressed(); // Check for next character
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (text_box->current_pos > 0) {
                text_box->current_pos--;
                text_box->textbox_text[text_box->current_line][text_box->current_pos] = '\0';
            } else if (text_box->current_line > 0) {
                // Move back to the previous line if current line is empty
                text_box->current_line--;
                text_box->current_pos = strlen(text_box->textbox_text[text_box->current_line]);
            }
        }
    }

    // Activate or deactivate the text box on mouse click
    Rectangle raylibRect = toRaylibRectangle(text_box->textbox_bounds);

    if (CheckCollisionPointRec(GetMousePosition(), raylibRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        text_box->active = true;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        text_box->active = false;
    }
}
// func to get text
char* get_text_box_content(rui *text_box) {
    static char full_text[MAX_LINES * MAX_LINE_LENGTH] = { 0 }; // Static buffer to store concatenated text
    memset(full_text, 0, sizeof(full_text)); // Clear previous content
    
    for (int i = 0; i < text_box->line_count; i++) {
        strcat(full_text, text_box->textbox_text[i]);
        if (i < text_box->line_count - 1) {
            strcat(full_text, "\n"); // Add newline between lines
        }
    }
    
    return full_text;
}

//====================================== entry ======================================

// Initialize a single-line text entry box
rui text_entry_ui(float x, float y, float width, float height, int font_size, RUI_COLOR text_color, RUI_COLOR background_color) {
    rui entry = {0};
    entry.textentry_bounds = (rui_rect){ x,y,width,height };
    entry.font_size = font_size;
    entry.text_color = text_color;
    entry.background_color = background_color;
    entry.active = false;
    entry.cursor_position = 0;
    entry.text_offset = 0;
    memset(entry.text, 0, MAX_TEXT_LENGTH); // Initialize text with empty characters
    return entry;
}

// Draws the single-line text entry box and handles cursor blinking
void render_text_entry(rui *entry) {
		// Convert rui_rect to Raylib Rectangle
	Rectangle raylibRect = toRaylibRectangle(entry->textentry_bounds);
    draw_rectangle_from_rect(raylibRect, entry->background_color); // Draw background
    draw_rectangle_lines(entry->textentry_bounds.x, entry->textentry_bounds.y, entry->textentry_bounds.width, entry->textentry_bounds.height, COLOR_DARKGRAY); // Draw border

    // Calculate visible text starting from the text offset
    char *visible_text = entry->text + entry->text_offset;
    
    draw_text(visible_text, entry->textentry_bounds.x + 5, entry->textentry_bounds.y + (entry->textentry_bounds.height - entry->font_size) / 2, entry->font_size, entry->text_color);

    // Draw blinking cursor if active
    if (entry->active) {
        int cursor_x = entry->textentry_bounds.x + 5 + MeasureText(visible_text, entry->font_size);
        if ((GetTime() * 2.0f) - (int)(GetTime() * 2.0f) < 0.5f) { // Blinking effect
            draw_rectangle(cursor_x, entry->textentry_bounds.y + (entry->textentry_bounds.height - entry->font_size) / 2, 2, entry->font_size, entry->text_color);
        }
    }
}

// Updates the text entry box with user input and handles text scrolling when full
void update_text_entry(rui *entry) {
    if (entry->active) {
        int key = GetCharPressed();

        while (key > 0) {
            // Check for printable characters and prevent overflow
            if (key >= 32 && key <= 125 && entry->cursor_position < MAX_TEXT_LENGTH - 1) {
                entry->text[entry->cursor_position] = (char)key;
                entry->cursor_position++;
                entry->text[entry->cursor_position] = '\0';

                // Calculate the text width and adjust the offset to keep text visible
                int text_width = MeasureText(entry->text + entry->text_offset, entry->font_size);
                if (text_width > entry->textentry_bounds.width - 10) {
                    entry->text_offset++;
                }
            }
            key = GetCharPressed();
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (entry->cursor_position > 0) {
                entry->cursor_position--;
                entry->text[entry->cursor_position] = '\0';

                // Adjust the text offset if there’s extra space after deleting
                int text_width = MeasureText(entry->text + entry->text_offset, entry->font_size);
                if (text_width < entry->textentry_bounds.width - 10 && entry->text_offset > 0) {
                    entry->text_offset--;
                }
            }
        }
    }

    // Activate or deactivate the text entry box on mouse click
	Rectangle raylibRect = toRaylibRectangle(entry->textentry_bounds);// Convert rui_rect to Raylib Rectangle

    if (CheckCollisionPointRec(GetMousePosition(),raylibRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        entry->active = true;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        entry->active = false;
    }
}
