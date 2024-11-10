#include "raylib.h"

typedef enum {
    WINDOW_MODE_FIXED,
    WINDOW_MODE_MOVABLE,
    WINDOW_MODE_RESIZABLE
} WindowMode;

typedef struct {
    Rectangle bounds;        // Window bounding box
    bool isDragging;         // Tracks if the window is being dragged
    bool isResizing;         // Tracks if the window is being resized
    Vector2 dragOffset;      // Offset for dragging
    WindowMode mode;         // Mode for the window (fixed, movable, resizable)
    bool movable;            // Option to allow or disallow movement
    bool showTitle;          // Option to show or hide title bar
    const char* title;       // Title text for the window
    bool scrollEnabled;      // Enables or disables scrolling
    float contentHeight;     // Total height of the content (for scrolling)
    float scrollOffset;      // Offset for scrolling
    Rectangle scrollBar;     // Scroll bar bounding box
    bool isScrollBarDragging;// Tracks if the scroll bar is being dragged
    float scrollBarDragOffset;// Offset for dragging the scroll bar
} MenuWindow;

MenuWindow create_menu_window(int x, int y, int width, int height, const char* title, WindowMode mode, bool showTitle, bool movable, bool scrollEnabled, float contentHeight) {
    MenuWindow window = {0};
    window.bounds = (Rectangle){ x, y, width, height };
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
    window.scrollBar = (Rectangle){ x + width - 15, y + 30, 15, (height - 30) * (height / contentHeight) };
    window.isScrollBarDragging = false;
    window.scrollBarDragOffset = 0;
    return window;
}

void update_menu_window(MenuWindow *window) {
    Vector2 mousePos = GetMousePosition();

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
            window->scrollBar.x = window->bounds.x + window->bounds.width - 15;
            window->scrollBar.y = window->bounds.y + 30 + (window->scrollOffset / (window->contentHeight - window->bounds.height + 30)) * (window->bounds.height - 30 - window->scrollBar.height);
        }
    }

    if (window->mode == WINDOW_MODE_RESIZABLE) {
        Rectangle resizeArea = { window->bounds.x + window->bounds.width - 10, window->bounds.y + window->bounds.height - 10, 10, 10 };
        if (CheckCollisionPointRec(mousePos, resizeArea) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            window->isResizing = true;
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) window->isResizing = false;

        if (window->isResizing) {
            window->bounds.width = mousePos.x - window->bounds.x;
            window->bounds.height = mousePos.y - window->bounds.y;

            if (window->bounds.width < 100) window->bounds.width = 100;
            if (window->bounds.height < 50) window->bounds.height = 50;
        }
    }

    if (window->scrollEnabled && window->contentHeight > window->bounds.height - 30) {
        float maxOffset = window->contentHeight - (window->bounds.height - 30);

        // Mouse wheel scroll
        window->scrollOffset -= GetMouseWheelMove() * 20;
        if (window->scrollOffset < 0) window->scrollOffset = 0;
        if (window->scrollOffset > maxOffset) window->scrollOffset = maxOffset;

        // Scroll bar dragging
        if (CheckCollisionPointRec(mousePos, window->scrollBar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            window->isScrollBarDragging = true;
            window->scrollBarDragOffset = mousePos.y - window->scrollBar.y;
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) window->isScrollBarDragging = false;

        if (window->isScrollBarDragging) {
            window->scrollBar.y = mousePos.y - window->scrollBarDragOffset;
            if (window->scrollBar.y < window->bounds.y + 30) window->scrollBar.y = window->bounds.y + 30;
            if (window->scrollBar.y + window->scrollBar.height > window->bounds.y + window->bounds.height)
                window->scrollBar.y = window->bounds.y + window->bounds.height - window->scrollBar.height;

            // Update scroll offset based on scroll bar position
            window->scrollOffset = ((window->scrollBar.y - window->bounds.y - 30) / (window->bounds.height - 30 - window->scrollBar.height)) * maxOffset;
        }

        // Update scroll bar height and position
        float visibleRatio = (window->bounds.height - 30) / window->contentHeight;
        window->scrollBar.height = (window->bounds.height - 30) * visibleRatio;
        window->scrollBar.x = window->bounds.x + window->bounds.width - 15;
        window->scrollBar.y = window->bounds.y + 30 + (window->scrollOffset / maxOffset) * (window->bounds.height - 30 - window->scrollBar.height);
    }
}

void render_menu_window(MenuWindow *window) {
    DrawRectangleRec(window->bounds, LIGHTGRAY);

    if (window->showTitle) {
        Rectangle titleBar = { window->bounds.x, window->bounds.y, window->bounds.width, 30 };
        DrawRectangleRec(titleBar, DARKGRAY);
        DrawText(window->title, window->bounds.x + 5, window->bounds.y + 5, 20, RAYWHITE);
    }

    if (window->mode == WINDOW_MODE_RESIZABLE) {
        DrawRectangle(window->bounds.x + window->bounds.width - 10, window->bounds.y + window->bounds.height - 10, 10, 10, DARKGRAY);
    }

    if (window->scrollEnabled && window->contentHeight > window->bounds.height - 30) {
        DrawRectangleRec(window->scrollBar, GRAY);
    }

    Rectangle clipArea = { window->bounds.x + 5, window->bounds.y + 35, window->bounds.width - 20, window->bounds.height - 40 };
    BeginScissorMode(clipArea.x, clipArea.y, clipArea.width, clipArea.height);

    float yOffset = 30 - window->scrollOffset;
    for (int i = 0; i < 10; i++) {
        float rectY = window->bounds.y + yOffset + i * 50;
        if (rectY >= clipArea.y && rectY <= clipArea.y + clipArea.height)
            DrawRectangle(window->bounds.x + 10, rectY, window->bounds.width - 30, 40, i % 2 == 0 ? BLUE : GREEN);
    }
    EndScissorMode();
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib - Menu Window with Scroll Bar");

    MenuWindow menuWindow = create_menu_window(200, 100, 300, 200, "Scrollable Window", WINDOW_MODE_RESIZABLE, true, true, true, 700);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        update_menu_window(&menuWindow);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        render_menu_window(&menuWindow);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
