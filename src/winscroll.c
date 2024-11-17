#include "raylib.h"

// Enum to define different modes for the menu window
typedef enum {
    WINDOW_MODE_FIXED,      // Window cannot be moved or resized
    WINDOW_MODE_MOVABLE,    // Window can be moved but not resized
    WINDOW_MODE_RESIZABLE   // Window can be moved and resized
} WindowMode;

// Struct to represent a menu window with all its properties
typedef struct {
    Rectangle bounds;        // Defines the window's position and size
    bool isDragging;         // Indicates if the window is being dragged
    bool isResizing;         // Indicates if the window is being resized
    Vector2 dragOffset;      // Offset between mouse and window position during dragging
    WindowMode mode;         // Mode of the window (fixed, movable, or resizable)
    bool movable;            // Determines if the window can be moved
    bool showTitle;          // Determines if the title bar is visible
    const char* title;       // Text displayed in the title bar
    bool scrollEnabled;      // Enables or disables scrolling functionality
    float contentHeight;     // Height of the window content for scrolling calculations
    float scrollOffset;      // Current scroll offset for content
    Rectangle scrollBar;     // Bounding box for the scroll bar
    bool isScrollBarDragging;// Indicates if the scroll bar is being dragged
    float scrollBarDragOffset;// Offset between mouse and scroll bar position during dragging
} MenuWindow;

// Function to create and initialize a menu window
MenuWindow create_menu_window(
    int x, int y, int width, int height, const char* title, WindowMode mode,
    bool showTitle, bool movable, bool scrollEnabled, float contentHeight
) {
    MenuWindow window = {0};
    window.bounds = (Rectangle){ x, y, width, height }; // Set position and size
    window.isDragging = false;
    window.isResizing = false;
    window.dragOffset = (Vector2){ 0, 0 };
    window.mode = mode;
    window.showTitle = showTitle;
    window.movable = movable;
    window.title = title;
    window.scrollEnabled = scrollEnabled;
    window.contentHeight = contentHeight;
    window.scrollOffset = 0;
    window.scrollBar = (Rectangle){
        x + width - 15,       // Position on the right of the window
        y + 30,               // Below the title bar
        15,                   // Width of the scroll bar
        (height - 30) * (height / contentHeight) // Height proportional to visible content
    };
    window.isScrollBarDragging = false;
    window.scrollBarDragOffset = 0;
    return window;
}

// Function to update the window's behavior based on user interaction
void update_menu_window(MenuWindow *window) {
    Vector2 mousePos = GetMousePosition(); // Get current mouse position

    // Handle window dragging if movable and title bar is visible
    if (window->movable && window->showTitle) {
        Rectangle titleBar = { window->bounds.x, window->bounds.y, window->bounds.width, 30 };
        if (CheckCollisionPointRec(mousePos, titleBar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            window->isDragging = true;
            window->dragOffset = (Vector2){ mousePos.x - window->bounds.x, mousePos.y - window->bounds.y };
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) window->isDragging = false;

        if (window->isDragging) {
            // Update window position
            window->bounds.x = mousePos.x - window->dragOffset.x;
            window->bounds.y = mousePos.y - window->dragOffset.y;
            // Adjust scroll bar position
            window->scrollBar.x = window->bounds.x + window->bounds.width - 15;
            window->scrollBar.y = window->bounds.y + 30 +
                (window->scrollOffset / (window->contentHeight - window->bounds.height + 30)) * 
                (window->bounds.height - 30 - window->scrollBar.height);
        }
    }

    // Handle window resizing if resizable
    if (window->mode == WINDOW_MODE_RESIZABLE) {
        Rectangle resizeArea = { window->bounds.x + window->bounds.width - 10, window->bounds.y + window->bounds.height - 10, 10, 10 };
        if (CheckCollisionPointRec(mousePos, resizeArea) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            window->isResizing = true;
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) window->isResizing = false;

        if (window->isResizing) {
            // Update window size
            window->bounds.width = mousePos.x - window->bounds.x;
            window->bounds.height = mousePos.y - window->bounds.y;

            // Enforce minimum size
            if (window->bounds.width < 100) window->bounds.width = 100;
            if (window->bounds.height < 50) window->bounds.height = 50;
        }
    }

    // Handle scrolling if enabled
    if (window->scrollEnabled && window->contentHeight > window->bounds.height - 30) {
        float maxOffset = window->contentHeight - (window->bounds.height - 30);

        // Scroll with mouse wheel
        window->scrollOffset -= GetMouseWheelMove() * 20;
        if (window->scrollOffset < 0) window->scrollOffset = 0;
        if (window->scrollOffset > maxOffset) window->scrollOffset = maxOffset;

        // Dragging the scroll bar
        if (CheckCollisionPointRec(mousePos, window->scrollBar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            window->isScrollBarDragging = true;
            window->scrollBarDragOffset = mousePos.y - window->scrollBar.y;
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) window->isScrollBarDragging = false;

        if (window->isScrollBarDragging) {
            // Update scroll bar position
            window->scrollBar.y = mousePos.y - window->scrollBarDragOffset;
            if (window->scrollBar.y < window->bounds.y + 30) window->scrollBar.y = window->bounds.y + 30;
            if (window->scrollBar.y + window->scrollBar.height > window->bounds.y + window->bounds.height)
                window->scrollBar.y = window->bounds.y + window->bounds.height - window->scrollBar.height;

            // Update scroll offset based on scroll bar position
            window->scrollOffset = ((window->scrollBar.y - window->bounds.y - 30) / 
                                    (window->bounds.height - 30 - window->scrollBar.height)) * maxOffset;
        }

        // Adjust scroll bar height and position dynamically
        float visibleRatio = (window->bounds.height - 30) / window->contentHeight;
        window->scrollBar.height = (window->bounds.height - 30) * visibleRatio;
        window->scrollBar.x = window->bounds.x + window->bounds.width - 15;
        window->scrollBar.y = window->bounds.y + 30 +
            (window->scrollOffset / maxOffset) * (window->bounds.height - 30 - window->scrollBar.height);
    }
}

// Function to render the menu window and its components
void render_menu_window(MenuWindow *window) {
    // Draw window background
    DrawRectangleRec(window->bounds, LIGHTGRAY);

    // Draw title bar if enabled
    if (window->showTitle) {
        Rectangle titleBar = { window->bounds.x, window->bounds.y, window->bounds.width, 30 };
        DrawRectangleRec(titleBar, DARKGRAY);
        DrawText(window->title, window->bounds.x + 5, window->bounds.y + 5, 20, RAYWHITE);
    }

    // Draw resize handle if resizable
    if (window->mode == WINDOW_MODE_RESIZABLE) {
        DrawRectangle(window->bounds.x + window->bounds.width - 10, window->bounds.y + window->bounds.height - 10, 10, 10, DARKGRAY);
    }

    // Draw scroll bar if scrolling is enabled
    if (window->scrollEnabled && window->contentHeight > window->bounds.height - 30) {
        DrawRectangleRec(window->scrollBar, GRAY);

        // Draw the scroll thumb's visible area
        float thumbY = window->scrollBar.y;
        float thumbHeight = window->scrollBar.height;
        DrawRectangle(window->scrollBar.x + 2, thumbY + 2, window->scrollBar.width - 4, thumbHeight - 4, DARKGRAY);
    }

    // Define clipping area for scrollable content
    Rectangle clipArea = { window->bounds.x + 5, window->bounds.y + 35, window->bounds.width - 20, window->bounds.height - 40 };
    BeginScissorMode(clipArea.x, clipArea.y, clipArea.width, clipArea.height);

    // Render content with scrolling offset
    float yOffset = 30 - window->scrollOffset;
    for (int i = 0; i < 10; i++) {
        float rectY = window->bounds.y + yOffset + i * 50;
        if (rectY + 40 >= clipArea.y && rectY <= clipArea.y + clipArea.height) {
            DrawRectangle(window->bounds.x + 10, rectY, window->bounds.width - 30, 40, i % 2 == 0 ? BLUE : GREEN);
            DrawText(TextFormat("Item %d", i + 1), window->bounds.x + 15, rectY + 10, 20, RAYWHITE);
        }
    }

    EndScissorMode();
}


void draw_rect(MenuWindow * menu){
 	
 	DrawRectangle(menu->bounds.x+(10), menu->bounds.y+(10), 300, 700, RED);
 }
// Main function to set up and run the application
int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib - Menu Window with Scroll Bar");
    // Create a menu window instance
    MenuWindow menu = create_menu_window(100,100, 400, 300, "Menu Window",WINDOW_MODE_RESIZABLE, true, true, true, 800);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Update and render the menu window
        update_menu_window(&menu);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        render_menu_window(&menu);
        // draw_rect(&menu);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
