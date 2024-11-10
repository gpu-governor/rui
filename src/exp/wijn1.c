#include "raylib.h"

typedef enum {
    WINDOW_MODE_FIXED,
    WINDOW_MODE_MOVABLE,
    WINDOW_MODE_RESIZABLE
} WindowMode;

typedef struct {
    Rectangle bounds;    // Window bounding box
    bool isDragging;     // Tracks if the window is being dragged
    bool isResizing;     // Tracks if the window is being resized
    Vector2 dragOffset;  // Offset for dragging
    WindowMode mode;     // Mode for the window (fixed, movable, resizable)
    bool movable;        // Option to allow or disallow movement
    bool showTitle;      // Option to show or hide title bar
    const char* title;   // Title text for the window
} MenuWindow;

// Function to create a menu window
MenuWindow create_menu_window(int x, int y, int width, int height, const char* title, WindowMode mode, bool showTitle, bool movable) {
    MenuWindow window = {0};
    window.bounds = (Rectangle){ x, y, width, height };
    window.isDragging = false;
    window.isResizing = false;
    window.dragOffset = (Vector2){ 0, 0 };
    window.mode = mode;
    window.showTitle = showTitle;
    window.movable = movable;
    window.title = title;
    return window;
}

// Function to handle updates (movement and resizing) for the window
void update_menu_window(MenuWindow *window) {
    Vector2 mousePos = GetMousePosition();

    // Handle dragging if the window is set to be movable
    if (window->movable && window->showTitle) {
        Rectangle titleBar = { window->bounds.x, window->bounds.y, window->bounds.width, 30 };

        if (CheckCollisionPointRec(mousePos, titleBar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            window->isDragging = true;
            window->dragOffset = (Vector2){ mousePos.x - window->bounds.x, mousePos.y - window->bounds.y };
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) window->isDragging = false;

        if (window->isDragging) {
            window->bounds.x = mousePos.x - window->dragOffset.x;
            window->bounds.y = mousePos.y - window->dragOffset.y;
        }
    }

    // Handle resizing if the window mode allows it
    if (window->mode == WINDOW_MODE_RESIZABLE) {
        Rectangle resizeArea = { window->bounds.x + window->bounds.width - 10, window->bounds.y + window->bounds.height - 10, 10, 10 };

        if (CheckCollisionPointRec(mousePos, resizeArea) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            window->isResizing = true;
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) window->isResizing = false;

        if (window->isResizing) {
            window->bounds.width = mousePos.x - window->bounds.x;
            window->bounds.height = mousePos.y - window->bounds.y;

            // Minimum window size constraints
            if (window->bounds.width < 100) window->bounds.width = 100;
            if (window->bounds.height < 50) window->bounds.height = 50;
        }
    }
}

// Function to render the menu window and its title bar
void render_menu_window(MenuWindow *window) {
    // Draw window background
    DrawRectangleRec(window->bounds, LIGHTGRAY);

    // Draw title bar if enabled
    if (window->showTitle) {
        Rectangle titleBar = { window->bounds.x, window->bounds.y, window->bounds.width, 30 };
        DrawRectangleRec(titleBar, DARKGRAY);
        DrawText(window->title, window->bounds.x + 5, window->bounds.y + 5, 20, RAYWHITE);
    }

    // Draw resizing handle if window is resizable
    if (window->mode == WINDOW_MODE_RESIZABLE) {
        DrawRectangle(window->bounds.x + window->bounds.width - 10, window->bounds.y + window->bounds.height - 10, 10, 10, DARKGRAY);
    }
}

int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib - Menu Window Example");

    // Create a resizable, movable window
    MenuWindow menuWindow = create_menu_window(200, 100, 300, 200, "Menu Window", WINDOW_MODE_RESIZABLE, true, true);

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update window based on input
        update_menu_window(&menuWindow);

        // Start Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render the menu window
        render_menu_window(&menuWindow);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
