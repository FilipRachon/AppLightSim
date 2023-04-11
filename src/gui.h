#pragma once
#include <vector>

#include "renderer.h"

enum class GUI_Call {
	None,
	FreezeCall,
	UnfreezeCall
};

enum class ButtonState {
	Normal,
	Highlighted,
	Held,
	Pressed
};

enum class SliderState {
	Normal,
	Highlighted,
	Held
};

class GUI_Element : public Drawable {
protected:
	Rect rect;

	bool isMouseInside(int x, int y) const;
public:
	GUI_Element(Rect r);
	
	virtual GUI_Call update(MouseState &mouseState) = 0;
};

class GUI_Slider : public GUI_Element {
protected:
	Sprite bar;
	Sprite knob;
	SliderState state;
	Rect barRect;
	Rect knobRect;
	float value;

	virtual void drawCall(Renderer &renderer) override;
	friend class Renderer;
public:
	GUI_Slider(Rect r, Texture &barTexture, Texture &knobTexture, float startVal = 0.5f);

	bool isSliding();
	void setValue(float val);
	float getValue();

	virtual GUI_Call update(MouseState &mouseState) override;
};

class GUI_Button : public GUI_Element {
protected:
	ButtonState state;
	Sprite sprite;

	virtual void drawCall(Renderer &renderer) override;
	friend class Renderer;
public:
	GUI_Button(Rect r, Texture &t);

	bool wasPressed();

	virtual GUI_Call update(MouseState &mouseState) override;
};

class GUI_ButtonMenu : public GUI_Button {
protected:
	std::vector<GUI_Button*> subButtons;

	virtual void drawCall(Renderer &renderer) override;
	friend class Renderer;
public:
	GUI_ButtonMenu(Rect r, Texture &t);

	void addSubButton(GUI_Button *b);
	bool wasPressed();
	
	virtual GUI_Call update(MouseState &mouseState) override;
};

class GUI : public Drawable {
	Window &window;
	std::vector<GUI_Element*> elements;

	GUI_Element *freezeCaller;
	bool frozen;

	virtual void drawCall(Renderer &renderer) override;
	friend class Renderer;
public:
	GUI(Window &win);
	
	void addElement(GUI_Element *el);
	void update();
};