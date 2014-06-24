/*
 * App.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "App.h"
#include "Delaunay.h"

#include <iostream>

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
				// Close window : exit
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
		//1150, 640 - tamanho sem overlay
		this->window.create(sf::VideoMode(1024, 768), "IPPD");

		StateManager::SetActiveState(STATE_MENU);

		retorno = true;
	} catch(int e) {
		retorno = false;
	}
	return retorno;
}

void App::AppEvent(sf::Event * event) {
	Eventos::AppEvent(event);
	StateManager::GetActiveState()->AppEvent(event);
}

void App::Closed() {
	this->window.close();
}
void App::KeyPressed(sf::Event::KeyEvent keyEvent) {
}
void App::KeyReleased(sf::Event::KeyEvent keyEvent) {

}

void App::AppLoop() {
	FPS::FPSControl.OnLoop();
	StateManager::GetActiveState()->OnLoop();
	this->window.setFramerateLimit(FPS::FPSControl.maxFrame);


}
void App::AppRender() {
	StateManager::GetActiveState()->OnRender(&window);
}
void App::AppCleanup() {

}
