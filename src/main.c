#include "rui.h"

int main() {
    init_ui("title", 800, 600);

    MenuWindow menu = create_menu_window(100, 100, 400, 300, "Menu Window", WINDOW_MODE_RESIZABLE, true, true, true, 800);

    // Button
    Button mybutton = create_button("Click Me", 20, 20);

    while (!WindowShouldClose()) {
        // Updates
        update_menu_window(&menu);
        update_button_position(&mybutton, &menu);

        if (update_button(&mybutton)) {
            // Button was clicked, handle the event
            TraceLog(LOG_INFO, "Button was clicked!");
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render Menu and Button
        render_menu_window(&menu);
        render_button(&mybutton);

        EndDrawing();
    }

    quit_ui();
    return 0;
}
