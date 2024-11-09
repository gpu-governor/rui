#include "raylib.h"
#include <string.h>

#define MAX_TEXT_LENGTH 256

typedef struct {
    Rectangle bounds;           // Position and size of the text entry box
    char text[MAX_TEXT_LENGTH];  // Text entered by the user
    int font_size;               // Font size for text display
    Color text_color;            // Text color
    Color background_color;      // Background color of the text box
    bool active;                 // True if the text box is selected
    int cursor_position;         // Current position of the cursor within the text
    int text_offset;             // Offset for scrolling text within bounds
} text_entry;

// Initialize a single-line text entry box
text_entry create_text_entry(float x, float y, float width, float height, int font_size, Color text_color, Color background_color) {
    text_entry entry = {0};
    entry.bounds = (Rectangle){ x, y, width, height };
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
void render_text_entry(text_entry *entry) {
    DrawRectangleRec(entry->bounds, entry->background_color); // Draw background
    DrawRectangleLines(entry->bounds.x, entry->bounds.y, entry->bounds.width, entry->bounds.height, DARKGRAY); // Draw border

    // Calculate visible text starting from the text offset
    const char *visible_text = entry->text + entry->text_offset;
    
    DrawText(visible_text, entry->bounds.x + 5, entry->bounds.y + (entry->bounds.height - entry->font_size) / 2, entry->font_size, entry->text_color);

    // Draw blinking cursor if active
    if (entry->active) {
        int cursor_x = entry->bounds.x + 5 + MeasureText(visible_text, entry->font_size);
        if ((GetTime() * 2.0f) - (int)(GetTime() * 2.0f) < 0.5f) { // Blinking effect
            DrawRectangle(cursor_x, entry->bounds.y + (entry->bounds.height - entry->font_size) / 2, 2, entry->font_size, entry->text_color);
        }
    }
}

// Updates the text entry box with user input and handles text scrolling when full
void update_text_entry(text_entry *entry) {
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
                if (text_width > entry->bounds.width - 10) {
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
                if (text_width < entry->bounds.width - 10 && entry->text_offset > 0) {
                    entry->text_offset--;
                }
            }
        }
    }

    // Activate or deactivate the text entry box on mouse click
    if (CheckCollisionPointRec(GetMousePosition(), entry->bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        entry->active = true;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        entry->active = false;
    }
}

int main(void) {
    InitWindow(800, 600, "Single-Line Text Entry with Scrolling");
    SetTargetFPS(60);

    text_entry entry = create_text_entry(100, 100, 600, 50, 20, BLACK, RAYWHITE);

    while (!WindowShouldClose()) {
        update_text_entry(&entry);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        render_text_entry(&entry);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
