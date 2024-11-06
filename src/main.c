#include"rui.h"
#include"backend_raylib.h"
int main (){
	init_ui("title",640,480);
	rui mybutton = button_ui("click me",20,20);
	style_ui(&DarkTheme);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RAYWHITE);
		render_button(&mybutton);
		EndDrawing();
	}
	quit_ui();
	return 0;
}
