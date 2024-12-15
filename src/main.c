#include "rui.h"

int main() {
    init_ui("title", 800, 600);

    MenuWindow menu = create_menu_window(100, 100, 400, 300, "Menu Window", WINDOW_MODE_RESIZABLE, true, true, true, 800);

    // Button
    Button mybutton = create_button("Click Me", 20, 20);
    TextBox mytextbox = create_text_box(40, 100, 300, 150, 14, COLOR_BLACK, COLOR_WHITE, "Enter your text here...");
    TextEntry entry = create_text_entry(20, 70, 300, 20, 14, COLOR_BLACK, COLOR_WHITE);

    while (!WindowShouldClose()) {
        // Updates
        update_menu_window(&menu);
        update_button_position(&mybutton, &menu); // working
        update_text_box(&mytextbox);
		update_textbox_position(&mytextbox, &menu); // working
        update_text_entry(&entry);
		update_entry_position(&entry,&menu);
        if (update_button(&mybutton)) {
            // Button was clicked, handle the event
            TraceLog(LOG_INFO, "Button was clicked!");
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render Menu and Button
        render_menu_window(&menu);
        render_button(&mybutton);
		render_text_box(&mytextbox);
		render_text_entry(&entry);


        EndDrawing();
    }

    quit_ui();
    return 0;
}
