#include"rui.h"

int main (){
	init_ui("title",800,600);
	Button mybutton = create_button("click me",20,20);
    TextBox mytextbox = create_text_box(40, 200, 600, 300, 14, COLOR_BLACK, COLOR_WHITE, "Enter your text here...");
    TextEntry entry = create_text_entry(20, 70, 600, 50, 20, COLOR_BLACK, COLOR_WHITE);

	// style_ui(&DarkTheme);
	while(!WindowShouldClose()){
	// all updates
		update_button(&mybutton);
        update_text_box(&mytextbox);
        update_text_entry(&entry);


		if (mybutton.is_clicked) {
		    // Button was clicked, handle the event here
		    TraceLog(LOG_INFO, "Button was clicked!");
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		render_text_box(&mytextbox);
		render_button(&mybutton);
		render_text_entry(&entry);
		EndDrawing();
	}
	quit_ui();
	return 0;
}
