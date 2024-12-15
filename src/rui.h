#include<stdint.h>
#include<stdbool.h>
#include"raylib.h"
#include"rl.h"
#include<string.h>

#define MAX_LINES 10           // for text boxes
#define MAX_LINE_LENGTH 128    // for text boxes
#define MAX_TEXT_LENGTH 256    // for text entry

//--------------------------- Style Struct ---------------------------

typedef struct {
    // Colors for various UI elements
    RUI_COLOR background_color;
    RUI_COLOR text_color;
    RUI_COLOR button_color;
    RUI_COLOR button_hover_color;
    RUI_COLOR button_active_color;
    RUI_COLOR border_color;
    RUI_COLOR frame_color;
    // Text properties
    float fontSize;
} rui_style;

//--------------------------- Rectangle Struct ---------------------------

typedef struct {
    float x;
    float y;
    float width;
    float height;
} rui_rect;

// Convert rui_rect to Raylib Rectangle
Rectangle toRaylibRectangle(rui_rect rect) {
    Rectangle raylibRect = { rect.x, rect.y, rect.width, rect.height };
    return raylibRect;
}
//--------------------------- window ---------------------------------

// Enum to define different modes for the menu window
typedef enum {
    WINDOW_MODE_FIXED,      // Window cannot be moved or resized
    WINDOW_MODE_MOVABLE,    // Window can be moved but not resized
    WINDOW_MODE_RESIZABLE   // Window can be moved and resized
} WindowMode;

// Struct to represent a menu window with all its properties
typedef struct {
	int x, y;
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
    window.x = x;
	window.y = y;
    window.bounds = (Rectangle){ window.x,window.y, width, height }; // Set position and size
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
    // float yOffset = 30 - window->scrollOffset;
    // for (int i = 0; i < 10; i++) {
    //     float rectY = window->bounds.y + yOffset + i * 50;
    //     if (rectY + 40 >= clipArea.y && rectY <= clipArea.y + clipArea.height) {
    //         DrawRectangle(window->bounds.x + 10, rectY, window->bounds.width - 30, 40, i % 2 == 0 ? BLUE : GREEN);
    //         DrawText(TextFormat("Item %d", i + 1), window->bounds.x + 15, rectY + 10, 20, RAYWHITE);
    //     }
    // }

    EndScissorMode();
}


void draw_rect(MenuWindow * menu){
 	
 	DrawRectangle(menu->bounds.x+(10), menu->bounds.y+(10), 300, 700, RED);
 }


//--------------------------- Button Struct ---------------------------

typedef struct {
    float x;
    float y;
    float width;
    float height;
    char *text;
    int font_size;
    RUI_COLOR button_color;
    RUI_COLOR text_color;
    rui_rect button_bounds;
    RUI_COLOR hover_color;
    RUI_COLOR clicked_color;
    bool is_hovered;
    bool is_clicked;

} Button;

Button create_button(char *text, float x, float y) {
    Button new_button = {0};
    new_button.text = text;
    new_button.x = x;
    new_button.y = y;
    new_button.width = 70;
    new_button.height = 30;
    new_button.font_size = 10;
    new_button.button_bounds = (rui_rect){ x, y, 70, 30 };
    new_button.button_color = COLOR_BLUE;
    new_button.hover_color = COLOR_SKYBLUE;
    new_button.clicked_color = COLOR_DARKBLUE;
    new_button.text_color = COLOR_WHITE;
    new_button.is_hovered = false;
    new_button.is_clicked = false;
    return new_button;
}

void render_button(Button *button) {
    RUI_COLOR draw_color = button->button_color;  // Updated to match `button_color`

    if (button->is_hovered) draw_color = button->hover_color;
    if (button->is_clicked) draw_color = button->clicked_color;

    float roundness = 0.2f;
    int segments = 10;


    // Convert rui_rect to Raylib Rectangle
    Rectangle raylibRect = toRaylibRectangle(button->button_bounds);

    // Use the Raylib Rectangle for drawing
    draw_rectangle_rounded(raylibRect, roundness, segments, draw_color);
    // Measure text width to center it horizontally
        int text_width = MeasureText(button->text, button->font_size);
        int text_x = button->button_bounds.x + (button->button_bounds.width - text_width) / 2;
        int text_y = button->button_bounds.y + (button->button_bounds.height - button->font_size) / 2;
    
        draw_text(button->text, text_x, text_y, button->font_size, button->text_color);
}

bool update_button(Button *button) {
    Vector2 mousePoint = GetMousePosition();

    // Convert rui_rect to Raylib Rectangle
    Rectangle raylibRect = toRaylibRectangle(button->button_bounds);

    button->is_hovered = CheckCollisionPointRec(mousePoint, raylibRect);
    button->is_clicked = false;

    if (button->is_hovered) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            button->is_clicked = true;
        }
        else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            return true; // Button was clicked
        }
    }
    return false; // Button was not clicked
}

void update_button_position(Button *button, MenuWindow *menu) {
    // Adjust button position relative to the menu
    button->button_bounds.x = menu->bounds.x + button->x;
    button->button_bounds.y = menu->bounds.y + button->y;
}

//--------------------------- Text Box Struct ---------------------------

typedef struct {
    rui_rect textbox_bounds;
    char textbox_text[MAX_LINES][MAX_LINE_LENGTH];
    int line_count;
    RUI_COLOR background_color;
    bool active;
    int current_line;
    int current_pos;
    const char *placeholder;
    RUI_COLOR placeholder_color;
    int font_size;
    RUI_COLOR text_color;
    float x,y;
} TextBox;

// Initialize a multiline text box with placeholder text
TextBox create_text_box(float x, float y, float width, float height, int font_size, RUI_COLOR text_color, RUI_COLOR background_color, const char *placeholder) {
    TextBox text_box = {0};
    text_box.x = x;
    text_box.y = y;
    text_box.textbox_bounds = (rui_rect){ text_box.x,text_box.y,width,height };
    text_box.font_size = font_size;
    text_box.text_color = text_color;
    text_box.background_color = background_color;
    text_box.line_count = 1;  // Start with one line
    text_box.current_line = 0;
    text_box.current_pos = 0;
    text_box.active = false;
    text_box.placeholder = placeholder;
    text_box.placeholder_color = COLOR_LIGHTGRAY;
    memset(text_box.textbox_text, 0, sizeof(text_box.textbox_text)); // Initialize text array with empty strings
    return text_box;
}

// Draws the multiline text box and handles cursor blinking
void render_text_box(TextBox *text_box) {
	// Convert rui_rect to Raylib Rectangle
	Rectangle raylibRect = toRaylibRectangle(text_box->textbox_bounds);
    draw_rectangle_from_rect(raylibRect, text_box->background_color);// Draw background
    draw_rectangle_lines(text_box->textbox_bounds.x, text_box->textbox_bounds.y, text_box->textbox_bounds.width, text_box->textbox_bounds.height, COLOR_DARKGRAY);// Draw border

    bool is_empty = (text_box->line_count == 1 && strlen(text_box->textbox_text[0]) == 0);

    // If text box is empty and not active, show placeholder text
    if (is_empty && !text_box->active) {
            draw_text(text_box->placeholder, text_box->textbox_bounds.x + 5, text_box->textbox_bounds.y + 5, text_box->font_size, text_box->placeholder_color);
    } else {
        // Draw each line of text
        for (int i = 0; i < text_box->line_count; i++) {
            int y_offset = i * (text_box->font_size + 5); // Adjust line spacing
            draw_text(text_box->textbox_text[i], text_box->textbox_bounds.x + 5, text_box->textbox_bounds.y + y_offset + 5, text_box->font_size, text_box->text_color);
        }

        // Draw blinking cursor if active
        if (text_box->active && text_box->current_line < MAX_LINES) {
            int cursor_x = text_box->textbox_bounds.x + 5 + MeasureText(text_box->textbox_text[text_box->current_line], text_box->font_size);
            int cursor_y = text_box->textbox_bounds.y + (text_box->current_line * (text_box->font_size + 5)) + 5;
            if ((GetTime() * 2.0f) - (int)(GetTime() * 2.0f) < 0.5f) {  // Blinking effect
                // DrawRectangle(cursor_x, cursor_y, 2, text_box->font_size, text_box->text_color);
                draw_rectangle(cursor_x, cursor_y, 2, text_box->font_size, text_box->text_color);
            }
        }
    }
}

// Updates the multiline text box with user input and handles automatic line wrapping
void update_text_box(TextBox *text_box) {
    if (text_box->active) {
        int key = GetCharPressed();

        while (key > 0) {
            // Check for printable characters and prevent overflow
            if (key >= 32 && key <= 125) {
                int text_width = MeasureText(text_box->textbox_text[text_box->current_line], text_box->font_size);
                
                // Check if adding the character would exceed the box width
                if (text_width + MeasureText("A", text_box->font_size) >= text_box->textbox_bounds.width - 10 && text_box->current_line < MAX_LINES - 1) {
                    // Move to the next line if current line width exceeds bounds
                    text_box->current_line++;
                    text_box->line_count++;
                    text_box->current_pos = 0;
                }

                // Add the character to the current line if within bounds
                if (text_box->current_pos < MAX_LINE_LENGTH - 1) {
                    text_box->textbox_text[text_box->current_line][text_box->current_pos] = (char)key;
                    text_box->current_pos++;
                    text_box->textbox_text[text_box->current_line][text_box->current_pos] = '\0';  // Null-terminate the string
                }
            } else if (key == KEY_ENTER || key == '\n') { // Move to a new line on Enter key
                if (text_box->line_count < MAX_LINES) {
                    text_box->current_line++;
                    text_box->line_count++;
                    text_box->current_pos = 0;
                }
            }
            key = GetCharPressed(); // Check for next character
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (text_box->current_pos > 0) {
                text_box->current_pos--;
                text_box->textbox_text[text_box->current_line][text_box->current_pos] = '\0';
            } else if (text_box->current_line > 0) {
                // Move back to the previous line if current line is empty
                text_box->current_line--;
                text_box->current_pos = strlen(text_box->textbox_text[text_box->current_line]);
            }
        }
    }

    // Activate or deactivate the text box on mouse click
    Rectangle raylibRect = toRaylibRectangle(text_box->textbox_bounds);

    if (CheckCollisionPointRec(GetMousePosition(), raylibRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        text_box->active = true;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        text_box->active = false;
    }
}
// func to get text
char* get_text_box_content(TextBox *text_box) {
    static char full_text[MAX_LINES * MAX_LINE_LENGTH] = { 0 }; // Static buffer to store concatenated text
    memset(full_text, 0, sizeof(full_text)); // Clear previous content
    
    for (int i = 0; i < text_box->line_count; i++) {
        strcat(full_text, text_box->textbox_text[i]);
        if (i < text_box->line_count - 1) {
            strcat(full_text, "\n"); // Add newline between lines
        }
    }
    
    return full_text;
}

void update_textbox_position(TextBox *text_box, MenuWindow *menu) {
    // Adjust button position relative to the menu
    text_box->textbox_bounds.x = menu->bounds.x + text_box->x;
    text_box->textbox_bounds.y = menu->bounds.y + text_box->y;
}
//--------------------------- Text Entry Struct ---------------------------

typedef struct {
    rui_rect textentry_bounds;
    char text[MAX_TEXT_LENGTH];
    int cursor_position;
    int text_offset;
    bool active;
    int font_size;
    RUI_COLOR text_color;
    RUI_COLOR background_color;
    float x, y;
} TextEntry;

// Initialize a single-line text entry box
TextEntry create_text_entry(float x, float y, float width, float height, int font_size, RUI_COLOR text_color, RUI_COLOR background_color) {
	TextEntry entry = {0};
	entry.x = x;
	entry.y = y;
    entry.textentry_bounds = (rui_rect){ x,y,width,height };
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
void render_text_entry(TextEntry *entry) {
		// Convert rui_rect to Raylib Rectangle
	Rectangle raylibRect = toRaylibRectangle(entry->textentry_bounds);
    draw_rectangle_from_rect(raylibRect, entry->background_color); // Draw background
    draw_rectangle_lines(entry->textentry_bounds.x, entry->textentry_bounds.y, entry->textentry_bounds.width, entry->textentry_bounds.height, COLOR_DARKGRAY); // Draw border

    // Calculate visible text starting from the text offset
    char *visible_text = entry->text + entry->text_offset;
    
    draw_text(visible_text, entry->textentry_bounds.x + 5, entry->textentry_bounds.y + (entry->textentry_bounds.height - entry->font_size) / 2, entry->font_size, entry->text_color);

    // Draw blinking cursor if active
    if (entry->active) {
        int cursor_x = entry->textentry_bounds.x + 5 + MeasureText(visible_text, entry->font_size);
        if ((GetTime() * 2.0f) - (int)(GetTime() * 2.0f) < 0.5f) { // Blinking effect
            draw_rectangle(cursor_x, entry->textentry_bounds.y + (entry->textentry_bounds.height - entry->font_size) / 2, 2, entry->font_size, entry->text_color);
        }
    }
}

// Updates the text entry box with user input and handles text scrolling when full
void update_text_entry(TextEntry *entry) {
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
                if (text_width > entry->textentry_bounds.width - 10) {
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
                if (text_width < entry->textentry_bounds.width - 10 && entry->text_offset > 0) {
                    entry->text_offset--;
                }
            }
        }
    }

    // Activate or deactivate the text entry box on mouse click
	Rectangle raylibRect = toRaylibRectangle(entry->textentry_bounds);// Convert rui_rect to Raylib Rectangle

    if (CheckCollisionPointRec(GetMousePosition(),raylibRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        entry->active = true;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        entry->active = false;
    }
}
void update_entry_position(TextEntry *entry, MenuWindow *menu) {
    // Adjust button position relative to the menu
    entry->textentry_bounds.x = menu->bounds.x + entry->x;
    entry->textentry_bounds.y = menu->bounds.y + entry->y;
}
//---------------------------- radio --------------------------------
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

// Function to create a single radio button
RadioButton create_radio_button(int x, int y, int width, int height, const char *label) {
    RadioButton button = { x, y, width, height, label, false };
    return button;
}

// Function to render a single radio button
void render_radio_button(RadioButton *radioButton) {
    draw_circle(radioButton->x, radioButton->y, radioButton->width / 2, COLOR_DARKGRAY);
    if (radioButton->isSelected) {
        draw_circle(radioButton->x, radioButton->y, radioButton->width / 4, COLOR_BLUE);
    }
    draw_text(radioButton->label, radioButton->x + radioButton->width, radioButton->y - radioButton->height / 2, 10, COLOR_BLACK);
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

void update_radio_position(RadioButton *radioButton, MenuWindow *menu) {
    // Adjust button position relative to the menu
    entry->textentry_bounds.x = menu->bounds.x + entry->x;
    entry->textentry_bounds.y = menu->bounds.y + entry->y;
}
//--------------------------- General UI Functions ---------------------------

void init_ui(char *title, int w, int h) {
    init(title, w, h);
}

void quit_ui() {
    quit();
}
