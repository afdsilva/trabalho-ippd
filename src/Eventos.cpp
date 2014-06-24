/*
 * Eventos.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "Eventos.h"

Eventos::Eventos() {

}
Eventos::~Eventos() {
}

void Eventos::AppEvent(sf::Event * event) {
	if (event->type == sf::Event::Closed) {
		Closed();
	}
	if (event->type == sf::Event::Resized) {
		Resized(event->size);
	}
	if (event->type == sf::Event::LostFocus) {
		LostFocus();
	}
	if (event->type == sf::Event::GainedFocus) {
		GainedFocus();
	}
	if (event->type == sf::Event::TextEntered) {
		TextEntered(event->text);
	}
	if (event->type == sf::Event::KeyPressed) {
		KeyPressed(event->key);
	}
	if (event->type == sf::Event::KeyReleased) {
		KeyReleased(event->key);
	}
	if (event->type == sf::Event::MouseWheelMoved) {
		MouseWheelMoved(event->mouseWheel);
	}
	if (event->type == sf::Event::MouseButtonPressed) {
		MouseButtonPressed(event->mouseButton);
	}
	if (event->type == sf::Event::MouseButtonReleased) {
		MouseButtonReleased(event->mouseButton);
	}
	if (event->type == sf::Event::MouseMoved) {
		MouseMoved(event->mouseMove);
	}
	if (event->type == sf::Event::MouseEntered) {
		MouseEntered();
	}
	if (event->type == sf::Event::MouseLeft) {
		MouseLeft();
	}
	if (event->type == sf::Event::JoystickButtonPressed) {
		JoystickButtonPressed(event->joystickButton);
	}
	if (event->type == sf::Event::JoystickButtonReleased) {
		JoystickButtonReleased(event->joystickButton);
	}
	if (event->type == sf::Event::JoystickMoved) {
		JoystickMoved(event->joystickMove);
	}
	if (event->type == sf::Event::JoystickConnected) {
		JoystickConnected(event->joystickConnect);
	}
	if (event->type == sf::Event::JoystickDisconnected) {
		JoystickDisconnected(event->joystickConnect);
	}
}
void Eventos::Closed() {
}
void Eventos::Resized(sf::Event::SizeEvent size) {
}
void Eventos::LostFocus() {
}
void Eventos::GainedFocus() {
}
void Eventos::TextEntered(sf::Event::TextEvent text) {
}
void Eventos::KeyPressed(sf::Event::KeyEvent keyEvent) {
}
void Eventos::KeyReleased(sf::Event::KeyEvent keyEvent) {
}
void Eventos::MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent) {
}
void Eventos::MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent) {
}
void Eventos::MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent) {
}
void Eventos::MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent) {
}
void Eventos::MouseEntered() {
}
void Eventos::MouseLeft() {
}
void Eventos::JoystickButtonPressed(sf::Event::JoystickButtonEvent joyButtonEvent) {
}
void Eventos::JoystickButtonReleased(sf::Event::JoystickButtonEvent joyButtonEvent) {
}
void Eventos::JoystickMoved(sf::Event::JoystickMoveEvent joyMoveEvent) {
}
void Eventos::JoystickConnected(sf::Event::JoystickConnectEvent joyConnectEvent) {
}
void Eventos::JoystickDisconnected(sf::Event::JoystickConnectEvent joyConnectEvent) {
}
