/*
 * App.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "App.h"

int main(int argc, char **argv) {

	App aPath ;
	return aPath.AppExec();

}

App::App() {
}

int App::AppExec() {
	int retorno = 0;
	try {
		if (AppInit() == false)
			throw 1;

		sf::Event event;
		while(this->window.isOpen()) {
			while(this->window.pollEvent(event)) {
				AppEvent(&event);
			}
			AppLoop();
			AppRender();
		}
		retorno = EXIT_SUCCESS;
	} catch(int e) {
		retorno = EXIT_FAILURE;
	}

	AppCleanup();

	return retorno;
}

bool App::AppInit() {
	bool retorno;
	try {
		this->window.create(sf::VideoMode(1024, 768), "IPPD - Delaunay Triangulation");

		StateManager::SetActiveState(STATE_MENU);

		retorno = true;
	} catch(exception & e) {
		std::cout << e.what() << std::endl;
		retorno = false;
	}
	return retorno;
}

void App::AppEvent(sf::Event * event) {
	try {
		if (StateManager::GetActiveState() == 0) {
			this->window.close();
			return;
		}
		Eventos::AppEvent(event);
		StateManager::GetActiveState()->AppEvent(event);
	} catch(exception & e) {
		std::cout << e.what() << std::endl;
	}
}

void App::AppLoop() {
	try {
		FPS::FPSControl.OnLoop();
		this->window.setFramerateLimit(FPS::FPSControl.maxFrame);
		if (StateManager::GetActiveState() == 0)
			return;
		StateManager::GetActiveState()->OnLoop();
	} catch(exception & e) {
		std::cout << e.what() << std::endl;
	}


}
void App::AppRender() {
	try {
		if (StateManager::GetActiveState() == 0)
			return;
		StateManager::GetActiveState()->OnRender(&window);
	} catch(exception & e) {
		std::cout << e.what() << std::endl;
	}
}
void App::AppCleanup() {
}

/**
 * Eventos
 */
void App::Closed() {
	this->window.close();
}
void App::KeyPressed(sf::Event::KeyEvent keyEvent) {
}
void App::KeyReleased(sf::Event::KeyEvent keyEvent) {

}
