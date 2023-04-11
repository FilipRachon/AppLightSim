#include "gui.h"

bool GUI_Element::isMouseInside(int x, int y) const 
{
	return (x >= rect.x && x <= rect.x + rect.w &&
		y >= rect.y && y <= rect.y + rect.h);
}

GUI_Element::GUI_Element(Rect r) 
{
	rect = r;
}

void GUI_Slider::drawCall(Renderer &renderer) 
{
	switch (state) {
	case SliderState::Normal:
		knob.setTextureTile(0, 3);
		break;
	case SliderState::Highlighted:
		knob.setTextureTile(1, 3);
		break;
	case SliderState::Held:
		knob.setTextureTile(2, 3);
		break;
	}

	bar.setRect(barRect);
	knob.setRect(knobRect);
	renderer.draw(bar);
	renderer.draw(knob);
}

GUI_Slider::GUI_Slider(Rect r, Texture &barTexture, Texture &knobTexture, float startVal) 
	: GUI_Element(r), bar(barTexture), knob(knobTexture) 
{
	state = SliderState::Normal;
	rect = r;
	barRect = {r.x, r.y + r.h / 4, r.w, r.h / 2};
	bar.setRect(barRect);
	knobRect = {r.x, r.y, r.h / 4, r.h};
	knob.setRect(knobRect);
	setValue(startVal);
}

float GUI_Slider::getValue()
{
	return value;
}

bool GUI_Slider::isSliding() 
{
	return state == SliderState::Held;
}

void GUI_Slider::setValue(float val) 
{
	if (val < 0.f)
		val = 0.f;
	if (val > 1.f)
		val = 1.f;
	value = val;
	knobRect.x = rect.x + value * (rect.w - knobRect.w);
	knob.setRect(knobRect);
}

GUI_Call GUI_Slider::update(MouseState &mouseState) 
{
	switch (state) {
	case SliderState::Normal:
		if (isMouseInside(mouseState.x, mouseState.y)) {
			state = SliderState::Highlighted;
		}
		/* FALL-THROUGH */
	case SliderState::Highlighted:
		if (!isMouseInside(mouseState.x, mouseState.y)) {
			state = SliderState::Normal;
			return GUI_Call::None;
		}
		if (!mouseState.leftPressed) {
			return GUI_Call::None;
		}
		state = SliderState::Held;
		/* FALL-THROUGH */
	case SliderState::Held:
		int x = (int)mouseState.x;
		float newValue = (float)(x - rect.x - (knobRect.w / 2)) / (rect.w - knobRect.w );
		setValue(newValue);

		if (mouseState.leftReleased) {
			if (isMouseInside(mouseState.x, mouseState.y))
				state = SliderState::Highlighted;
			else
				state = SliderState::Normal;
			return GUI_Call::UnfreezeCall;
		}
		return GUI_Call::FreezeCall;
	}
	return GUI_Call::None;
}


void GUI_Button::drawCall(Renderer &renderer) 
{
	switch (state) {
	case ButtonState::Normal:
		sprite.setTextureTile(0, 3);
		break;
	case ButtonState::Highlighted:
	case ButtonState::Pressed:
		sprite.setTextureTile(1, 3);
		break;
	case ButtonState::Held:
		sprite.setTextureTile(2, 3);
		break;
	}
	
	renderer.draw(sprite);
}

GUI_Button::GUI_Button(Rect r, Texture &t) 
	: GUI_Element(r), sprite(t) 
{
	state = ButtonState::Normal;
	sprite.setRect(rect);
}

GUI_Call GUI_Button::update(MouseState &mouseState) 
{
	switch (state) {
	case ButtonState::Normal:
		if (isMouseInside(mouseState.x, mouseState.y)) {
			state = ButtonState::Highlighted;
		}
		/* FALL-THROUGH */
	case ButtonState::Highlighted:
		if (!isMouseInside(mouseState.x, mouseState.y)) {
			state = ButtonState::Normal;
			return GUI_Call::None;
		}
		if (mouseState.leftPressed) {
			state = ButtonState::Held;
			return GUI_Call::FreezeCall;
		}
		
		return GUI_Call::None;

	case ButtonState::Held:
		if (mouseState.leftReleased) {
			if (isMouseInside(mouseState.x, mouseState.y)) {
				state = ButtonState::Pressed;
			} else {
				state = ButtonState::Normal;
			}
			return GUI_Call::UnfreezeCall;
		}
		return GUI_Call::None;

	case ButtonState::Pressed:
		state = ButtonState::Highlighted;
		return GUI_Call::None;
	}
	return GUI_Call::None;
}

bool GUI_Button::wasPressed() 
{
	return state == ButtonState::Pressed;
}


void GUI_ButtonMenu::drawCall(Renderer &renderer) 
{
	switch (state) {
	case ButtonState::Normal:
		sprite.setTextureTile(0, 3);
		break;
	case ButtonState::Highlighted:
	case ButtonState::Pressed:
		sprite.setTextureTile(1, 3);
		break;
	case ButtonState::Held:
		sprite.setTextureTile(2, 3);
		break;
	}
	
	renderer.draw(sprite);
	
	if (state == ButtonState::Pressed) {
		for (auto &b : subButtons)
			renderer.draw(*b);
	}
}

GUI_ButtonMenu::GUI_ButtonMenu(Rect r, Texture &t) 
	: GUI_Button(r, t) 
{
}

void GUI_ButtonMenu::addSubButton(GUI_Button *b) 
{
	subButtons.push_back(b);
}

GUI_Call GUI_ButtonMenu::update(MouseState &mouseState) 
{
	for (auto &b : subButtons) {
		b->update(mouseState);
	}

	switch (state) {
	case ButtonState::Normal:
		if (isMouseInside(mouseState.x, mouseState.y)) {
			state = ButtonState::Highlighted;
		}
		/* FALL-THROUGH */
	case ButtonState::Highlighted:
		if (!isMouseInside(mouseState.x, mouseState.y)) {
			state = ButtonState::Normal;
			return GUI_Call::None;
		}
		if (mouseState.leftPressed) {
			state = ButtonState::Held;
			return GUI_Call::FreezeCall;
		}
		
		return GUI_Call::None;

	case ButtonState::Held:
		if (mouseState.leftReleased) {
			if (isMouseInside(mouseState.x, mouseState.y)) {
				state = ButtonState::Pressed;
			} else {
				state = ButtonState::Normal;
				return GUI_Call::UnfreezeCall;
			}
		}
		return GUI_Call::None;

	case ButtonState::Pressed:
		for (auto &b : subButtons) {
			if (b->wasPressed()) {
				state = ButtonState::Highlighted;
				return GUI_Call::UnfreezeCall;
			}
		}
		return GUI_Call::None;
	}
	return GUI_Call::None;
}

bool GUI_ButtonMenu::wasPressed() 
{
	return state == ButtonState::Pressed;
}

void GUI::drawCall(Renderer &renderer) 
{
	for (auto el : elements) {
		renderer.draw(*el);
	}
}


GUI::GUI(Window &win) : window(win) 
{
	frozen = false;
	freezeCaller = nullptr;
}
	
void GUI::addElement(GUI_Element *el)
{
	elements.push_back(el);
}

void GUI::update() 
{
	MouseState mouseState = window.getMouseState();

	if (frozen) {
		if (freezeCaller->update(mouseState) == GUI_Call::UnfreezeCall) {
			frozen = false;
			freezeCaller = nullptr;
		}
		return;
	}

	for (auto el : elements) {
		if (el->update(mouseState) == GUI_Call::FreezeCall) {
			frozen = true;
			freezeCaller = el;
		}
	}
}