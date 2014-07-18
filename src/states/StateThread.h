/*
 * StateThread.h
 *
 *  Created on: 16/07/2014
 *      Author: andref
 */

#ifndef STATETHREAD_H_
#define STATETHREAD_H_

#include <SFML/Graphics.hpp>
#include <thread>

#include "State.h"
#include "../gui/Entidade.h"
#include "../Graph.h"
#include "../Quadtree.h"
#include "../delaunay_threads/DelaunayThread.h"

#include "../App.h"

class StateThread: public State {
private:
	static StateThread instance;
private:
	StateThread();

	int n_Threads;
	int n_Entries;
	int n_Depth;
	VerticeList m_Vertices;
	std::list<Vertice> m_VerticeList;
	TriangleList m_Triangles;
	Quadtree * quadTree;
	Graph * m_Mesh;
	std::mutex mutex;

public:
	void OnActivation();
	void OnDeactivation();
	void OnLoop();
	void OnRender(sf::RenderWindow * window);

public:
	static StateThread * GetIntance();

	void KeyPressed(sf::Event::KeyEvent keyEvent);
	void MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent);
	void MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent);
	void MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent);
	void MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent);
};

#endif /* STATETHREAD_H_ */
