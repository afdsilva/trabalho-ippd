/*
 * StateTBB.h
 *
 *  Created on: 16/07/2014
 *      Author: andref
 */

#ifndef STATETBB_H_
#define STATETBB_H_

#include <SFML/Graphics.hpp>

#include "State.h"
#include "../gui/Entidade.h"
#include "../Graph.h"
#include "../delaunay_tbb/DelaunayTBB.h"

#include "../App.h"

class StateTBB: public State {
private:
	static StateTBB instance;
private:
	StateTBB();
	TriangleList m_Triangles;

public:
	void OnActivation();
	void OnDeactivation();
	void OnLoop();
	void OnRender(sf::RenderWindow * window);

public:
	static StateTBB * GetIntance();

	void KeyPressed(sf::Event::KeyEvent keyEvent);
	void MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent);
	void MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent);
	void MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent);
	void MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent);
};

#endif /* STATETBB_H_ */
