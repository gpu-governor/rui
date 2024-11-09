#include"rui.h"

int main (){
	init_ui("title",800,600);
	// buttons
	Button mybutton = create_button("click me",20,20);
	// text boxs
    TextBox mytextbox = create_text_box(40, 200, 600, 300, 14, COLOR_BLACK, COLOR_WHITE, "Enter your text here...");
    TextEntry entry = create_text_entry(20, 70, 600, 50, 20, COLOR_BLACK, COLOR_WHITE);
	// radio
	// Create individual radio buttons
	    RadioButton button1 = create_radio_button(100, 100, 20, 20, "Option 1");
	    RadioButton button2 = create_radio_button(100, 150, 20, 20, "Option 2");
	    RadioButton button3 = create_radio_button(100, 200, 20, 20, "Option 3");
	    
	    RadioButton button4 = create_radio_button(100, 400, 20, 20, "Option 4");
	    RadioButton button5 = create_radio_button(100, 450, 20, 20, "Option 5");
	
	    // Group the radio buttons
	    RadioButton buttons[] = { button1, button2, button3 };
	    RadioButtonGroup group = create_radio_button_group(buttons, 3);
	
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

		// Update and render radio button group
		        update_radio_button_group(&group);
		        render_radio_button_group(&group);
		
		        // Update and render individual radio buttons
		        update_radio_button(&button4);
		        update_radio_button(&button5);
		        render_radio_button(&button4);
		        render_radio_button(&button5);
		
		EndDrawing();
	}
	quit_ui();
	return 0;
}
