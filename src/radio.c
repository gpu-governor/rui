#include "raylib.h"

// RadioButton structure definition
typedef struct {
    int x;
    int y;
    int width;
    int height;
    const char *label;
    bool isSelected;
} RadioButton;

// RadioButtonGroup structure definition
typedef struct {
    RadioButton *buttons;
    int count;
    int selectedOption; // Index of the selected option in the group, -1 if none selected
} RadioButtonGroup;

// Function prototypes
RadioButton create_radio_button(int x, int y, int width, int height, const char *label);
void render_radio_button(RadioButton *radioButton);
void update_radio_button(RadioButton *radioButton);
RadioButtonGroup create_radio_button_group(RadioButton *buttons, int count);
void render_radio_button_group(RadioButtonGroup *group);
void update_radio_button_group(RadioButtonGroup *group);
int get_selected_option(RadioButtonGroup *group);

// Main function
int main() {
    InitWindow(800, 600, "Radio Button Example");

    // Create individual radio buttons
    RadioButton button1 = create_radio_button(100, 100, 20, 20, "Option 1");
    RadioButton button2 = create_radio_button(100, 150, 20, 20, "Option 2");
    RadioButton button3 = create_radio_button(100, 200, 20, 20, "Option 3");
    
    RadioButton button4 = create_radio_button(100, 400, 20, 20, "Option 4");
    RadioButton button5 = create_radio_button(100, 450, 20, 20, "Option 5");

    // Group the radio buttons
    RadioButton buttons[] = { button1, button2, button3 };
    RadioButtonGroup group = create_radio_button_group(buttons, 3);

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Update and render radio button group
        update_radio_button_group(&group);
        render_radio_button_group(&group);

        // Update and render individual radio buttons
        update_radio_button(&button4);
        update_radio_button(&button5);
        render_radio_button(&button4);
        render_radio_button(&button5);

        // Display the selected option
        int selected = get_selected_option(&group);
        if (selected != -1) {
            DrawText(TextFormat("Selected option: %s", buttons[selected].label), 300, 100, 20, BLACK);
        } else {
            DrawText("No option selected", 300, 100, 20, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Function to create a single radio button
RadioButton create_radio_button(int x, int y, int width, int height, const char *label) {
    RadioButton button = { x, y, width, height, label, false };
    return button;
}

// Function to render a single radio button
void render_radio_button(RadioButton *radioButton) {
    DrawCircle(radioButton->x, radioButton->y, radioButton->width / 2, DARKGRAY);
    if (radioButton->isSelected) {
        DrawCircle(radioButton->x, radioButton->y, radioButton->width / 4, BLUE);
    }
    DrawText(radioButton->label, radioButton->x + radioButton->width, radioButton->y - radioButton->height / 2, 10, BLACK);
}

// Function to update the state of a single radio button
void update_radio_button(RadioButton *radioButton) {
    Rectangle bounds = { radioButton->x - radioButton->width / 2, radioButton->y - radioButton->height / 2, radioButton->width, radioButton->height };
    if (CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Toggle selection state
        radioButton->isSelected = !radioButton->isSelected;
    }
}

// Function to create a radio button group
RadioButtonGroup create_radio_button_group(RadioButton *buttons, int count) {
    RadioButtonGroup group = { buttons, count, -1 };
    return group;
}

// Function to render a radio button group
void render_radio_button_group(RadioButtonGroup *group) {
    for (int i = 0; i < group->count; i++) {
        render_radio_button(&group->buttons[i]);
    }
}

// Function to update a radio button group
void update_radio_button_group(RadioButtonGroup *group) {
    for (int i = 0; i < group->count; i++) {
        Rectangle bounds = { group->buttons[i].x - group->buttons[i].width / 2, 
                             group->buttons[i].y - group->buttons[i].height / 2, 
                             group->buttons[i].width, 
                             group->buttons[i].height };
        if (CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (group->selectedOption == i) {
                // Deselect if the currently selected option is clicked again
                group->selectedOption = -1;
                group->buttons[i].isSelected = false;
            } else {
                group->selectedOption = i;
                for (int j = 0; j < group->count; j++) {
                    group->buttons[j].isSelected = (j == i);
                }
            }
        }
    }
}

// Function to get the selected option index in a group
int get_selected_option(RadioButtonGroup *group) {
    return group->selectedOption;
}
