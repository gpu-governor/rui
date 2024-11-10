#include "raylib.h"

typedef struct {
    Rectangle bounds;   // Slider bounding box
    int minValue;       // Minimum value of the slider
    int maxValue;       // Maximum value of the slider
    int value;          // Current value
    bool isDragging;    // Is slider being dragged
    bool showValue;     // Flag to show or hide the value
    int handleSize;     // Handle width for easier adjustment
} Slider;

// Function to create a slider with default parameters
Slider create_slider(int x, int y, int width, int height, int minValue, int maxValue, int startValue, bool showValue) {
    Slider slider = {0};
    slider.bounds = (Rectangle){ x, y, width, height };
    slider.minValue = minValue;
    slider.maxValue = maxValue;
    slider.value = startValue;
    slider.isDragging = false;
    slider.showValue = showValue;
    slider.handleSize = 15;  // Increased handle size for better visibility
    return slider;
}

// Function to render the slider and handle input
void render_slider(Slider *slider) {
    // Handle mouse interaction
    Vector2 mousePos = GetMousePosition();

    // Check if the mouse is over the slider
    bool isMouseOver = CheckCollisionPointRec(mousePos, slider->bounds);

    if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        slider->isDragging = true;
    }
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        slider->isDragging = false;
    }

    // Update slider value while dragging
    if (slider->isDragging) {
        float relativePos = (mousePos.x - slider->bounds.x) / slider->bounds.width;
        slider->value = slider->minValue + (int)((slider->maxValue - slider->minValue) * relativePos);
        if (slider->value < slider->minValue) slider->value = slider->minValue;
        if (slider->value > slider->maxValue) slider->value = slider->maxValue;
    }

    // Draw the slider background
    DrawRectangleRec(slider->bounds, LIGHTGRAY);

    // Draw the slider "handle" based on the value
    float handlePosX = slider->bounds.x + ((float)(slider->value - slider->minValue) / (slider->maxValue - slider->minValue)) * slider->bounds.width;
    DrawRectangle(handlePosX - (slider->handleSize / 2), slider->bounds.y, slider->handleSize, slider->bounds.height, DARKGRAY);

    // Draw the slider value if showValue is true
    if (slider->showValue) {
        const char* valueText = TextFormat("%d", slider->value);
        int textWidth = MeasureText(valueText, 20);
        DrawText(valueText, slider->bounds.x + (slider->bounds.width / 2) - (textWidth / 2), slider->bounds.y + (slider->bounds.height / 2) - 10, 20, BLACK);
    }
}

int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib - Slider Widget Example");

    // Create a slider with the value display enabled
    Slider slider = create_slider(300, 200, 200, 20, 0, 100, 50, true);

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Start Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render the slider and handle input
        render_slider(&slider);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
