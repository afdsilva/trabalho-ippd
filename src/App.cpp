/*
 * App.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "App.h"
#include "Delaunay.h"

#include <iostream>
#include <thread>         // std::thread
#include <vector>

class point2d;

typedef std::vector<point2d> vetPoint;
typedef std::vector<point2d>::iterator itPoint;

class point2d {
public:
	int x,y;
	point2d() : x(0), y(0) {}
	point2d(int nx,int ny) {
		x = nx;
		y = ny;
	}
	void operator() (const point2d & p) {
		if (this->x == p.x && this->y == p.y) {
			cout << "encontrou (" << p.x << "," << p.y << ")" << endl;
		}
	}
};

void meuTesteThread(itPoint begin, itPoint end, int thread) {
	for(itPoint it = begin; it != end; it++) {
		point2d p = *it;
		cout << "Thread " << thread << " Processando(" << p.x << "," << p.y << ")" << endl;
		sf::sleep(sf::seconds(0.1));
	}

}

int main(int argc, char **argv) {

	App aPath ;
	return aPath.AppExec();
//
//	int n_threads = 10;
//	int n_points = 100;
//	std::vector<point2d> vetor;
//	for (int p = 0; p < n_points; p++) {
//		int x = rand() % 640;
//		int y = rand() % 480;
//		vetor.push_back(point2d(x,y));
//	}
//	//for_each(vetor.begin(), vetor.end(), point2d(5,5));
//	cout << "Fim" << endl;
//	int n = n_points / n_threads;
//	std::vector<std::thread *> vetorThreads;
//	for (int i = 0; i < n_threads; i++) {
//		itPoint itbegin = vetor.begin() + n * i;
//		itPoint itend = vetor.begin() + n * (i + 1);
//		std::thread * t = new std::thread(meuTesteThread,itbegin,itend,i);
//		vetorThreads.push_back(t);
//		//std::thread t (
//		//std::thread first (foo);     // spawn new thread that calls foo()
//		//std::thread second (bar,0);  // spawn new thread that calls bar(0)
//	}
//	for (int i = 0; i < n_threads; i++)
//		vetorThreads[i]->join();
//	//first.join();                // pauses until first finishes
//	//second.join();               // pauses until second finishes

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
