/*
 * StateMenu.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef STATEMENU_H_
#define STATEMENU_H_

#include <SFML/Graphics.hpp>
#include <SFML/System/Thread.hpp>

#include <string>
#include <vector>
#include <list>

#include "State.h"
#include "../gui/Entidade.h"
#include "../Graph.h"
#include "../delaunay_threads/DelaunayThread.h"

class StateMenu : public State {
private:
	static StateMenu instance;
private:
	StateMenu();
	int n_Threads;
	VerticeList m_Vertices;
	std::list<Vertice> m_VerticeList;
	TriangleList draw_Triangles;
	TriangleList m_Triangles;
	std::mutex mutex;
	sf::Clock clock;
	Button * nThreadsText;
	Button * nTrianglesText;
	Button * nTimeText;
	Button * nListSize;
	Graph * m_Mesh;
	std::vector<std::thread> threads;
	std::vector<sf::Thread *> sfThreads;


	bool cavity = false;
	bool autoDelaunay = false;
	bool mouseRight = false;
	bool mouseLeft = false;

public:
	void OnActivation();
	void OnDeactivation();
	void OnLoop();
	void OnRender(sf::RenderWindow * window);

public:
	static StateMenu * GetIntance();

	void KeyPressed(sf::Event::KeyEvent keyEvent);
	void MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent);
	void MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent);
	void MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent);
	void MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent);
};

#endif /* STATEMENU_H_ */
