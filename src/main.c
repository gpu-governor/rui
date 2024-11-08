#include"rui.h"
int main (){
	init_ui("title",800,600);
	rui mybutton = button_ui("click me",20,20);
    rui mytextbox = text_box_ui(40, 70, 600, 300, 14, COLOR_BLACK, COLOR_WHITE, "Enter your text here...");

	// style_ui(&DarkTheme);
	while(!WindowShouldClose()){
	// all updates
		update_button(&mybutton);
        update_text_box(&mytextbox);


		if (mybutton.is_clicked) {
		    // Button was clicked, handle the event here
		    TraceLog(LOG_INFO, "Button was clicked!");
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		render_text_box(&mytextbox);
		render_button(&mybutton);
		EndDrawing();
	}
	quit_ui();
	return 0;
}
