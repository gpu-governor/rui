#include"rui.h"

int main (){
	init_ui("title",800,600);
	MenuWindow menu = create_menu_window(100,100, 400, 300, "Menu Window",WINDOW_MODE_RESIZABLE, true, true, true, 800);

	// buttons
	Button mybutton = create_button("click me",20,20);

	// style_ui(&DarkTheme);
	while(!WindowShouldClose()){
	// all updates

        update_menu_window(&menu);
		update_button_postition(&mybutton,&menu);

		if (mybutton.is_clicked) {
		    // Button was clicked, handle the event here
		    TraceLog(LOG_INFO, "Button was clicked!");
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		render_button(&mybutton);
        render_menu_window(&menu);


		
		EndDrawing();
	}
	quit_ui();
	return 0;
}

