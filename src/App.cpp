/*
 * App.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "App.h"

int App::n_Threads = 2;
int App::n_Entries = 1000;

int main(int argc, char **argv) {
	int threads = App::n_Threads;
	int entries = App::n_Entries;
	int activestate = STATE_GUI;
	std::string filename;
	bool nogui = false;
	if (argc > 1)
		threads = std::atoi(argv[1]);
	if (argc > 2)
		entries = std::atoi(argv[2]);
	if (argc > 2) {
		for (int i = 0; i < argc; i++) {
			std::string s = argv[i];
			if (s.find("--tbb") != std::string::npos ) {
				activestate = STATE_TBB;
			} else if (s.find("--c++11") != std::string::npos ) {
				activestate = STATE_THREAD;
			} else if (s.find("--nogui") != std::string::npos ) {
				nogui = true;
			} else if (s.find("--file") != std::string::npos ) {
				if (argc >= i+1)
					filename = argv[i+1];
				else
					std::cout << "Argumentos insuficientes" << std::endl;
			}
		}

	}
	if (nogui) {
		VerticeList m_Vertices;
		switch(activestate) {
		case STATE_GUI:
			std::cout << "Modo não suportado" << std::endl;
			return EXIT_FAILURE;
			break;
		case STATE_TBB:
			if (filename.empty())
				GenerateVertices(entries,m_Vertices);
			else
				GenerateVertices(filename,m_Vertices);
			DelaunayTBB::ExecuteTBB(m_Vertices,threads);
			break;
		case STATE_THREAD:
			if (filename.empty())
				GenerateVertices(entries,m_Vertices);
			DelaunayThread::ExecuteThread(m_Vertices,threads,3);
			break;
		}
		return EXIT_SUCCESS;
	} else {
		App aPath ;
		return aPath.AppExec(threads,entries);
	}

}

App::App() {
}

int App::AppExec(int threads, int entries) {
	int retorno = 0;
	try {
		App::n_Threads = threads;
		App::n_Entries = entries;

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
		StateManager::SetActiveState(STATE_GUI);

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
