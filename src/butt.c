#include "raylib.h"

typedef struct Button {
    Rectangle bounds;
    Color color;
    Color hoverColor;
    Color clickColor;
    bool isHovered;
    bool isClicked;
} Button;

Button CreateButton(float x, float y, float width, float height, Color color, Color hoverColor, Color clickColor) {
    Button button = {0};
    button.bounds = (Rectangle){ x, y, width, height };
    button.color = color;
    button.hoverColor = hoverColor;
    button.clickColor = clickColor;
    button.isHovered = false;
    button.isClicked = false;
    return button;
}

bool UpdateButton(Button *button) {
    Vector2 mousePoint = GetMousePosition();
    button->isHovered = CheckCollisionPointRec(mousePoint, button->bounds);
    button->isClicked = false;

    if (button->isHovered) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            button->isClicked = true;
        }
        else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            return true; // Button was clicked
        }
    }
    return false; // Button was not clicked
}

void DrawButton(Button button) {
    Color drawColor = button.color;
    if (button.isHovered) drawColor = button.hoverColor;
    if (button.isClicked) drawColor = button.clickColor;

    DrawRectangleRounded(button.bounds, 0.2f, 10, drawColor);
    DrawText("Click Me", button.bounds.x + 20, button.bounds.y + 10, 20, WHITE);
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Button Example");

    Button myButton = CreateButton(350, 200, 100, 50, BLUE, SKYBLUE, DARKBLUE);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Update
        UpdateButton(&myButton);

        if (myButton.isClicked) {
            // Button was clicked, handle the event here
            TraceLog(LOG_INFO, "Button was clicked!");
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawButton(myButton);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
