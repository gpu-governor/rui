#include "raylib.h"

typedef enum {
    PROGRESS_MODE_AUTOMATIC,
    PROGRESS_MODE_MANUAL
} ProgressMode;

typedef struct {
    Rectangle bounds;    // Progress bar bounding box
    int minValue;        // Minimum value of the progress bar
    int maxValue;        // Maximum value of the progress bar
    int value;           // Current progress value
    bool showValue;      // Flag to show or hide the percentage value
    ProgressMode mode;   // Mode of the progress bar (automatic or manual)
    bool isDragging;     // Tracks if the progress bar is being dragged
} ProgressBar;

// Function to create a progress bar with default parameters
ProgressBar create_progress_bar(int x, int y, int width, int height, int minValue, int maxValue, int startValue, bool showValue, ProgressMode mode) {
    ProgressBar progressBar = {0};
    progressBar.bounds = (Rectangle){ x, y, width, height };
    progressBar.minValue = minValue;
    progressBar.maxValue = maxValue;
    progressBar.value = startValue;
    progressBar.showValue = showValue;
    progressBar.mode = mode;
    progressBar.isDragging = false;
    return progressBar;
}

// Function to update the progress bar value based on the mode
void update_progress_bar(ProgressBar *progressBar) {
    if (progressBar->mode == PROGRESS_MODE_AUTOMATIC) {
        // Increment progress automatically
        progressBar->value += 1;
        if (progressBar->value > progressBar->maxValue) {
            progressBar->value = progressBar->minValue;
        }
    } else if (progressBar->mode == PROGRESS_MODE_MANUAL) {
        // Check if mouse is over the progress bar
        Vector2 mousePos = GetMousePosition();
        bool isMouseOver = CheckCollisionPointRec(mousePos, progressBar->bounds);

        if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            progressBar->isDragging = true;
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            progressBar->isDragging = false;
        }

        // Update progress value based on mouse position while dragging
        if (progressBar->isDragging) {
            float relativePos = (mousePos.x - progressBar->bounds.x) / progressBar->bounds.width;
            progressBar->value = progressBar->minValue + (int)((progressBar->maxValue - progressBar->minValue) * relativePos);
            if (progressBar->value < progressBar->minValue) progressBar->value = progressBar->minValue;
            if (progressBar->value > progressBar->maxValue) progressBar->value = progressBar->maxValue;
        }
    }
}

// Function to render the progress bar and optionally show the percentage
void render_progress_bar(ProgressBar *progressBar) {
    // Ensure value stays within min and max range
    if (progressBar->value < progressBar->minValue) progressBar->value = progressBar->minValue;
    if (progressBar->value > progressBar->maxValue) progressBar->value = progressBar->maxValue;

    // Calculate the width of the filled part of the progress bar
    float progressWidth = ((float)(progressBar->value - progressBar->minValue) / (progressBar->maxValue - progressBar->minValue)) * progressBar->bounds.width;

    // Draw the background of the progress bar
    DrawRectangleRec(progressBar->bounds, LIGHTGRAY);

    // Draw the filled part of the progress bar
    DrawRectangle(progressBar->bounds.x, progressBar->bounds.y, progressWidth, progressBar->bounds.height, DARKGREEN);

    // Draw the percentage text if showValue is true
    if (progressBar->showValue) {
        int percentage = (int)(((float)(progressBar->value - progressBar->minValue) / (progressBar->maxValue - progressBar->minValue)) * 100);
        const char* valueText = TextFormat("%d%%", percentage);
        int textWidth = MeasureText(valueText, 20);
        DrawText(valueText, progressBar->bounds.x + (progressBar->bounds.width / 2) - (textWidth / 2), progressBar->bounds.y + (progressBar->bounds.height / 2) - 10, 20, BLACK);
    }
}

int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib - Progress Bar Example");

    // Create a progress bar with manual dragging enabled
    ProgressBar progressBar = create_progress_bar(300, 200, 200, 30, 0, 100, 0, true, PROGRESS_MODE_MANUAL);
    ProgressBar progressBar2 = create_progress_bar(300, 300, 200, 30, 0, 100, 0, true, PROGRESS_MODE_AUTOMATIC);

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update progress bar based on its mode
        update_progress_bar(&progressBar);
        update_progress_bar(&progressBar2);

        // Start Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render the progress bar
        render_progress_bar(&progressBar);
        render_progress_bar(&progressBar2);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
