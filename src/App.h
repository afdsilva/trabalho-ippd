/*
 * App.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef APP_H_
#define APP_H_

#include "gui/StateManager.h"
#include "gui/Eventos.h"
#include "gui/FPS.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <exception>

using namespace std;

class App : public Eventos {
private:
	sf::RenderWindow window;

public:
	App();
	int AppExec();

private:
	bool AppInit();
	void AppEvent(sf::Event * event);
		virtual void Closed();
		virtual void KeyPressed(sf::Event::KeyEvent keyEvent);
		virtual void KeyReleased(sf::Event::KeyEvent keyEvent);
	void AppLoop();
	void AppRender();
	void AppCleanup();

};

#endif /* APP_H_ */
