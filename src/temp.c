#include "raylib.h"
#include <string.h>

#define MAX_LINES 10
#define MAX_LINE_LENGTH 128

typedef struct {
    Rectangle bounds;                // Position and size of the text box
    char textbox_text[MAX_LINES][MAX_LINE_LENGTH]; // 2D array to store text lines
    int line_count;                  // Current line count in the box
    int font_size;                   // Font size for text display
    Color text_color;                // Text color
    Color background_color;          // Background color of the text box
    bool active;                     // True when the text box is selected
    int current_line;                // Current line being edited
    int current_pos;                 // Current position in the current line
    const char *placeholder;         // Placeholder text
    Color placeholder_color;         // Placeholder text color
} rui;

// Initialize a multiline text box with placeholder text
rui text_box_ui(float x, float y, float width, float height, int font_size, Color text_color, Color background_color, const char *placeholder) {
    rui text_box = {0};
    text_box.bounds = (Rectangle){ x, y, width, height };
    text_box.font_size = font_size;
    text_box.text_color = text_color;
    text_box.background_color = background_color;
    text_box.line_count = 1;  // Start with one line
    text_box.current_line = 0;
    text_box.current_pos = 0;
    text_box.active = false;
    text_box.placeholder = placeholder;
    text_box.placeholder_color = LIGHTGRAY;
    memset(text_box.textbox_text, 0, sizeof(text_box.textbox_text)); // Initialize text array with empty strings
    return text_box;
}

// Draws the multiline text box and handles cursor blinking
void render_text_box(rui *text_box) {
    DrawRectangleRec(text_box->bounds, text_box->background_color); // Draw background
    DrawRectangleLines(text_box->bounds.x, text_box->bounds.y, text_box->bounds.width, text_box->bounds.height, DARKGRAY); // Draw border

    bool is_empty = (text_box->line_count == 1 && strlen(text_box->textbox_text[0]) == 0);

    // If text box is empty and not active, show placeholder text
    if (is_empty && !text_box->active) {
        DrawText(text_box->placeholder, text_box->bounds.x + 5, text_box->bounds.y + 5, text_box->font_size, text_box->placeholder_color);
    } else {
        // Draw each line of text
        for (int i = 0; i < text_box->line_count; i++) {
            int y_offset = i * (text_box->font_size + 5); // Adjust line spacing
            DrawText(text_box->textbox_text[i], text_box->bounds.x + 5, text_box->bounds.y + y_offset + 5, text_box->font_size, text_box->text_color);
        }

        // Draw blinking cursor if active
        if (text_box->active && text_box->current_line < MAX_LINES) {
            int cursor_x = text_box->bounds.x + 5 + MeasureText(text_box->textbox_text[text_box->current_line], text_box->font_size);
            int cursor_y = text_box->bounds.y + (text_box->current_line * (text_box->font_size + 5)) + 5;
            if ((GetTime() * 2.0f) - (int)(GetTime() * 2.0f) < 0.5f) {  // Blinking effect
                DrawRectangle(cursor_x, cursor_y, 2, text_box->font_size, text_box->text_color);
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
                if (text_width + MeasureText("A", text_box->font_size) >= text_box->bounds.width - 10 && text_box->current_line < MAX_LINES - 1) {
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
    if (CheckCollisionPointRec(GetMousePosition(), text_box->bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        text_box->active = true;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        text_box->active = false;
    }
}

int main(void) {
    InitWindow(800, 600, "Multiline Text Box with Placeholder");
    SetTargetFPS(60);

    rui text_box = text_box_ui(100, 100, 600, 300, 14, BLACK, RAYWHITE, "Enter your text here...");

    while (!WindowShouldClose()) {
        update_text_box(&text_box);

        BeginDrawing();
        ClearBackground(RAYWHITE);
       	render_text_box(&text_box);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
