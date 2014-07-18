/*
 * StateGUI.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef STATEGUI_H_
#define STATEGUI_H_

#include <SFML/Graphics.hpp>
#include <SFML/System/Thread.hpp>

#include <string>
#include <vector>
#include <list>

#include "State.h"
#include "../gui/Entidade.h"
#include "../Graph.h"
#include "../Quadtree.h"
#include "../delaunay_threads/DelaunayThread.h"

#include "../App.h"

class StateGUI : public State {
private:
	static StateGUI instance;
private:
	sf::RenderWindow * m_Window;
	StateGUI();
	int n_Depth;
	std::list<Vertice> m_VerticeList;
	TriangleList draw_Triangles;
	TriangleList m_Triangles;
	std::mutex mutex;
	sf::Clock clock;
	sf::Time totalElapsed;

	Quadtree * quadTree;

	Button * nThreadsText;
	Button * nThreadsPlus;
	Button * nThreadsMinus;
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
	static StateGUI * GetIntance();

	void KeyPressed(sf::Event::KeyEvent keyEvent);
	void MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent);
	void MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent);
	void MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent);
	void MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent);
};

#endif /* STATEMENU_H_ */
