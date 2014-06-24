/*
 * Eventos.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef EVENTOS_H_
#define EVENTOS_H_

#include <SFML/Window/Event.hpp>

class Eventos {
public:
	Eventos();
	virtual ~Eventos();

	virtual void AppEvent(sf::Event * event);

	virtual void Closed();
	virtual void Resized(sf::Event::SizeEvent size);
	virtual void LostFocus();
	virtual void GainedFocus();
	virtual void TextEntered(sf::Event::TextEvent text);
	virtual void KeyPressed(sf::Event::KeyEvent keyEvent);
	virtual void KeyReleased(sf::Event::KeyEvent keyEvent);
	virtual void MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent);
	virtual void MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent);
	virtual void MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent);
	virtual void MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent);
	virtual void MouseEntered();
	virtual void MouseLeft();
	virtual void JoystickButtonPressed(sf::Event::JoystickButtonEvent joyButtonEvent);
	virtual void JoystickButtonReleased(sf::Event::JoystickButtonEvent joyButtonEvent);
	virtual void JoystickMoved(sf::Event::JoystickMoveEvent joyMoveEvent);
	virtual void JoystickConnected(sf::Event::JoystickConnectEvent joyConnectEvent);
	virtual void JoystickDisconnected(sf::Event::JoystickConnectEvent joyConnectEvent);

};
#endif /* EVENTOS_H_ */
