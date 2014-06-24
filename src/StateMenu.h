/*
 * StateMenu.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef STATEMENU_H_
#define STATEMENU_H_

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Entidade.h"
#include "Delaunay.h"

class StateMenu : public State {
private:
	static StateMenu instance;
private:
	StateMenu();

	sf::View view;

	vertexSet m_Vertices;
	triangleSet m_Triangles;
	triangle t;
	edgeSet m_Edges;
	bool mouseButton;
	sf::Vector2f mouseDrag;

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
