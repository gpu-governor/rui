#include "raylib.h"
#include <string.h>

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 40
#define OPTION_HEIGHT 30
#define MAX_OPTIONS 5

typedef struct {
    int x, y;
    int width, height;
    bool expanded;
    const char* options[MAX_OPTIONS];
    int selectedIndex;
} DropDown;

void DropDown_Init(DropDown* dropdown, int x, int y, const char* options[], int numOptions) {
    dropdown->x = x;
    dropdown->y = y;
    dropdown->width = BUTTON_WIDTH;
    dropdown->height = BUTTON_HEIGHT;
    dropdown->expanded = false;
    dropdown->selectedIndex = -1;

    for (int i = 0; i < numOptions && i < MAX_OPTIONS; ++i) {
        dropdown->options[i] = options[i];
    }
}

void DropDown_Render(DropDown* dropdown) {
    // Draw the main dropdown button
    Rectangle buttonRect = { dropdown->x, dropdown->y, dropdown->width, dropdown->height };
    DrawRectangleRec(buttonRect, (dropdown->expanded) ? DARKGRAY : LIGHTGRAY);
    
    // Draw the selected option or default text
    if (dropdown->selectedIndex >= 0) {
        DrawText(dropdown->options[dropdown->selectedIndex], dropdown->x + 10, dropdown->y + 10, 20, WHITE);
    } else {
        DrawText("Select an option", dropdown->x + 10, dropdown->y + 10, 20, WHITE);
    }

    // Draw the dropdown triangle icon
    Vector2 trianglePos = { dropdown->x + dropdown->width - 20, dropdown->y + dropdown->height / 2 };
    if (dropdown->expanded) {
        DrawTriangle(
            (Vector2) { trianglePos.x - 5, trianglePos.y + 5 },
            (Vector2) { trianglePos.x + 5, trianglePos.y + 5 },
            (Vector2) { trianglePos.x, trianglePos.y - 5 },
            WHITE
        );
    } else {
        DrawTriangle(
            (Vector2) { trianglePos.x - 5, trianglePos.y - 5 },
            (Vector2) { trianglePos.x + 5, trianglePos.y - 5 },
            (Vector2) { trianglePos.x, trianglePos.y + 5 },
            WHITE
        );
    }

    // If expanded, draw each option
    if (dropdown->expanded) {
        for (int i = 0; i < MAX_OPTIONS; ++i) {
            Rectangle optionRect = { dropdown->x, dropdown->y + dropdown->height + (i * OPTION_HEIGHT), dropdown->width, OPTION_HEIGHT };
            DrawRectangleRec(optionRect, (i == dropdown->selectedIndex) ? SKYBLUE : LIGHTGRAY);
            DrawText(dropdown->options[i], dropdown->x + 10, optionRect.y + 5, 20, BLACK);
        }
    }
}

void DropDown_HandleEvent(DropDown* dropdown) {
    // Check if main button is clicked to toggle expanded state
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        Rectangle buttonRect = { dropdown->x, dropdown->y, dropdown->width, dropdown->height };
        
        if (CheckCollisionPointRec(mousePos, buttonRect)) {
            dropdown->expanded = !dropdown->expanded;
        }
        // Handle option selection when dropdown is expanded
        else if (dropdown->expanded) {
            for (int i = 0; i < MAX_OPTIONS; ++i) {
                Rectangle optionRect = { dropdown->x, dropdown->y + dropdown->height + (i * OPTION_HEIGHT), dropdown->width, OPTION_HEIGHT };
                if (CheckCollisionPointRec(mousePos, optionRect)) {
                    dropdown->selectedIndex = i;
                    dropdown->expanded = false; // Collapse dropdown after selection
                    break;
                }
            }
        }
    }
}

int main(void) {
    // Initialize Raylib
    InitWindow(800, 600, "Dropdown Menu with Raylib");
    SetTargetFPS(60);

    // Dropdown options
    const char* options[] = { "Option 1", "Option 2", "Option 3", "Option 4", "Option 5" };
    DropDown dropdown;
    DropDown_Init(&dropdown, 220, 220, options, 5);

    // Main loop
    while (!WindowShouldClose()) {
        // Update
        DropDown_HandleEvent(&dropdown);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DropDown_Render(&dropdown);

        // Display the selected option below the dropdown
        if (dropdown.selectedIndex >= 0) {
            DrawText(TextFormat("Selected: %s", dropdown.options[dropdown.selectedIndex]), 220, 450, 20, DARKGRAY);
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
