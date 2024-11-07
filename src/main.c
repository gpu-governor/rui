#include"rui.h"
#include"rl.h"
int main (){
	init_ui("title",640,480);
	rui mybutton = button_ui("click me",20,20);
	// style_ui(&DarkTheme);
	while(!WindowShouldClose()){
		update_button(&mybutton);
		
		if (mybutton.is_clicked) {
		    // Button was clicked, handle the event here
		    TraceLog(LOG_INFO, "Button was clicked!");
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		render_button(&mybutton);
		EndDrawing();
	}
	quit_ui();
	return 0;
}
