#include "app.h"
#include "gui.h"
#include "canvas.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 500

App::App(int argc, char *argv[])
{
	if (argc > 1) {
		if (argv[1][0] == '-')
			return;
		// prevent warning (unused variables)
	}
}

void App::run()
{
	Window window("AppLightSim", SCREEN_WIDTH, SCREEN_HEIGHT);
	Renderer renderer(window);

	Texture 
		t1(renderer, "data/no_light.bmp"),
		t2(renderer, "data/mid_light.bmp"),
		t3(renderer, "data/full_light.bmp"),
		t4(renderer, "data/red_light.bmp"),
		t5(renderer, "data/green_light.bmp"),
		t6(renderer, "data/blue_light.bmp"),
		t7(renderer, "data/add.bmp"),
		t8(renderer, "data/remove.bmp"),
		t9(renderer, "data/triangle.bmp"),
		t10(renderer, "data/square.bmp"),
		t11(renderer, "data/circle.bmp"),
		t12(renderer, "data/bar_narrow.bmp"),
		t13(renderer, "data/bar_wide.bmp"),
		t14(renderer, "data/knob_small.bmp"),
		t15(renderer, "data/knob_big.bmp");

	GUI gui(window);

	GUI_Button button1({414, 20, 48, 48}, t1);
	GUI_Button button2({476, 20, 48, 48}, t2);
	GUI_Button button3({538, 20, 48, 48}, t3);
	gui.addElement(&button2);
	gui.addElement(&button1);
	gui.addElement(&button3);

	GUI_Button button4({414, 82, 48, 48}, t4);
	GUI_Button button5({414, 144, 48, 48}, t5);
	GUI_Button button6({414, 206, 48, 48}, t6);
	gui.addElement(&button4);
	gui.addElement(&button5);
	gui.addElement(&button6);

	GUI_ButtonMenu menu({414, 268, 48, 48}, t7);
	GUI_Button sub1({414, 330, 48, 48}, t9);
	GUI_Button sub2({476, 330, 48, 48}, t10);
	GUI_Button sub3({538, 330, 48, 48}, t11);
	menu.addSubButton(&sub1);
	menu.addSubButton(&sub2);
	menu.addSubButton(&sub3);
	gui.addElement(&menu);

	GUI_Button buttonClear({538, 268, 48, 48}, t8);
	gui.addElement(&buttonClear);

	GUI_Slider slider1({470,  82, 128, 48}, t12, t14);
	GUI_Slider slider2({470, 144, 128, 48}, t12, t14);
	GUI_Slider slider3({470, 206, 128, 48}, t12, t14);
	GUI_Slider slider4({20,  420, 380, 60}, t13, t15);
	gui.addElement(&slider1);
	gui.addElement(&slider2);
	gui.addElement(&slider3);
	gui.addElement(&slider4);

	Canvas canvas(renderer, window);
	canvas.setLightColor(0x808080FF);
	canvas.setLightRadius(150.f);

	while (!window.shouldExit()) {
		window.processEvents();

		gui.update();

		if (button1.wasPressed()) {
			canvas.setLightColor(0x000000FF);
			slider1.setValue(0.f);
			slider2.setValue(0.f);
			slider3.setValue(0.f);
			slider4.setValue(0.f);
			canvas.setLightRadius(slider4.getValue() * 300.f);
		}
		if (button2.wasPressed()) {
			canvas.setLightColor(0x808080FF);
			slider1.setValue(.5f);
			slider2.setValue(.5f);
			slider3.setValue(.5f);
			slider4.setValue(.5f);
			canvas.setLightRadius(slider4.getValue() * 300.f);
		}
		if (button3.wasPressed()) {
			canvas.setLightColor(0xFFFFFFFF);
			slider1.setValue(1.f);
			slider2.setValue(1.f);
			slider3.setValue(1.f);
			slider4.setValue(1.f);
			canvas.setLightRadius(slider4.getValue() * 300.f);
		}
		if (button4.wasPressed()) {
			canvas.setLightColor(0xFF0000FF);
			slider1.setValue(1.f);
			slider2.setValue(0.f);
			slider3.setValue(0.f);
		}
		if (button5.wasPressed()) {
			canvas.setLightColor(0x00FF00FF);
			slider1.setValue(0.f);
			slider2.setValue(1.f);
			slider3.setValue(0.f);
		}
		if (button6.wasPressed()) {
			canvas.setLightColor(0x0000FFFF);
			slider1.setValue(0.f);
			slider2.setValue(0.f);
			slider3.setValue(1.f);
		}

		if (slider1.isSliding() || slider2.isSliding() || slider3.isSliding()) {
			uint8_t R = slider1.getValue() * 255.f;
			uint8_t G = slider2.getValue() * 255.f;
			uint8_t B = slider3.getValue() * 255.f;
			uint8_t A = 0xFF;
			Color color = R<<24 | G<<16 | B<<8 | A;
			canvas.setLightColor(color);
		}

		if (slider4.isSliding()) {
			canvas.setLightRadius(slider4.getValue() * 300.f);
		}
		
		if (sub1.wasPressed())
			canvas.addTriangle();
		if (sub2.wasPressed())
			canvas.addSquare();
		if (sub3.wasPressed())
			canvas.addCircle();

		if (buttonClear.wasPressed())
			canvas.removeShapes();

		canvas.update();

		renderer.clear();

		renderer.draw(canvas);
		renderer.draw(gui);

		renderer.display();
	}
}