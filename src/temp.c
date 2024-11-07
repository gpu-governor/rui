#include "raylib.h"
#include <string.h>

#define MAX_LINES 10
#define MAX_LINE_LENGTH 128

typedef struct {
    Rectangle bounds;                // Position and size of the text box
    char text[MAX_LINES][MAX_LINE_LENGTH]; // 2D array to store text lines
    int line_count;                  // Current line count in the box
    int font_size;                   // Font size for text display
    Color text_color;                // Text color
    Color background_color;          // Background color of the text box
    bool active;                     // True when the text box is selected
    int current_line;                // Current line being edited
    int current_pos;                 // Current position in the current line
    const char *placeholder;         // Placeholder text
    Color placeholder_color;         // Placeholder text color
} MultiLineTextBox;

// Initialize a multiline text box with placeholder text
MultiLineTextBox CreateMultiLineTextBox(float x, float y, float width, float height, int font_size, Color text_color, Color background_color, const char *placeholder) {
    MultiLineTextBox box = {0};
    box.bounds = (Rectangle){ x, y, width, height };
    box.font_size = font_size;
    box.text_color = text_color;
    box.background_color = background_color;
    box.line_count = 1;  // Start with one line
    box.current_line = 0;
    box.current_pos = 0;
    box.active = false;
    box.placeholder = placeholder;
    box.placeholder_color = LIGHTGRAY;
    memset(box.text, 0, sizeof(box.text)); // Initialize text array with empty strings
    return box;
}

// Draws the multiline text box and handles cursor blinking
void DrawMultiLineTextBox(MultiLineTextBox *box) {
    DrawRectangleRec(box->bounds, box->background_color); // Draw background
    DrawRectangleLines(box->bounds.x, box->bounds.y, box->bounds.width, box->bounds.height, DARKGRAY); // Draw border

    bool is_empty = (box->line_count == 1 && strlen(box->text[0]) == 0);

    // If text box is empty and not active, show placeholder text
    if (is_empty && !box->active) {
        DrawText(box->placeholder, box->bounds.x + 5, box->bounds.y + 5, box->font_size, box->placeholder_color);
    } else {
        // Draw each line of text
        for (int i = 0; i < box->line_count; i++) {
            int y_offset = i * (box->font_size + 5); // Adjust line spacing
            DrawText(box->text[i], box->bounds.x + 5, box->bounds.y + y_offset + 5, box->font_size, box->text_color);
        }

        // Draw blinking cursor if active
        if (box->active && box->current_line < MAX_LINES) {
            int cursor_x = box->bounds.x + 5 + MeasureText(box->text[box->current_line], box->font_size);
            int cursor_y = box->bounds.y + (box->current_line * (box->font_size + 5)) + 5;
            if ((GetTime() * 2.0f) - (int)(GetTime() * 2.0f) < 0.5f) {  // Blinking effect
                DrawRectangle(cursor_x, cursor_y, 2, box->font_size, box->text_color);
            }
        }
    }
}

// Updates the multiline text box with user input and handles automatic line wrapping
void UpdateMultiLineTextBox(MultiLineTextBox *box) {
    if (box->active) {
        int key = GetCharPressed();

        while (key > 0) {
            // Check for printable characters and prevent overflow
            if (key >= 32 && key <= 125) {
                int text_width = MeasureText(box->text[box->current_line], box->font_size);
                
                // Check if adding the character would exceed the box width
                if (text_width + MeasureText("A", box->font_size) >= box->bounds.width - 10 && box->current_line < MAX_LINES - 1) {
                    // Move to the next line if current line width exceeds bounds
                    box->current_line++;
                    box->line_count++;
                    box->current_pos = 0;
                }

                // Add the character to the current line if within bounds
                if (box->current_pos < MAX_LINE_LENGTH - 1) {
                    box->text[box->current_line][box->current_pos] = (char)key;
                    box->current_pos++;
                    box->text[box->current_line][box->current_pos] = '\0';  // Null-terminate the string
                }
            } else if (key == KEY_ENTER || key == '\n') { // Move to a new line on Enter key
                if (box->line_count < MAX_LINES) {
                    box->current_line++;
                    box->line_count++;
                    box->current_pos = 0;
                }
            }
            key = GetCharPressed(); // Check for next character
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (box->current_pos > 0) {
                box->current_pos--;
                box->text[box->current_line][box->current_pos] = '\0';
            } else if (box->current_line > 0) {
                // Move back to the previous line if current line is empty
                box->current_line--;
                box->current_pos = strlen(box->text[box->current_line]);
            }
        }
    }

    // Activate or deactivate the text box on mouse click
    if (CheckCollisionPointRec(GetMousePosition(), box->bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        box->active = true;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        box->active = false;
    }
}

int main(void) {
    InitWindow(800, 600, "Multiline Text Box with Placeholder");

    MultiLineTextBox textBox = CreateMultiLineTextBox(100, 100, 600, 300, 20, BLACK, RAYWHITE, "Enter your text here...");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateMultiLineTextBox(&textBox);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawMultiLineTextBox(&textBox);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
