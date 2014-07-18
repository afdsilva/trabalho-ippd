/*
 * StateTBB.cpp
 *
 *  Created on: 16/07/2014
 *      Author: andref
 */

#include "StateTBB.h"
#include "../gui/StateManager.h"

StateTBB StateTBB::instance;

StateTBB::StateTBB() {
}
StateTBB * StateTBB::GetIntance() {
	return & instance;
}
void StateTBB::OnActivation() {
	ResourceFonts::loadFonts();
	Entidade::entityList.clear();
	Button::buttonList.clear();

	m_Triangles = DelaunayTBB::ExecuteTBB(Vertice::listVertices, App::n_Threads);
}

void StateTBB::OnDeactivation() {
	Entidade::entityList.clear();
}
void StateTBB::OnLoop() {

}
void StateTBB::OnRender(sf::RenderWindow * window) {
	std::for_each(m_Triangles.begin(), m_Triangles.end(), [&window](Triangle * t) {
		if (t) {
			sf::VertexArray shape(sf::Triangles,3);
			Vertice a = Edge::CommonVertice(t->getE2(),t->getE0()); //CA AB
			Vertice b = Edge::CommonVertice(t->getE0(),t->getE1()); //AB BC
			Vertice c = Edge::CommonVertice(t->getE1(),t->getE2()); //BC CA
			shape[0] = sf::Vertex(sf::Vector2f(a.x,a.y));
			shape[0].color = sf::Color::Blue;
			shape[1] = sf::Vertex(sf::Vector2f(b.x,b.y));
			shape[1].color = sf::Color::Red;
			shape[2] = sf::Vertex(sf::Vector2f(c.x,c.y));
			shape[2].color = sf::Color::Green;
			window->draw(shape);
		}
	});
	std::for_each(Vertice::listVertices.begin(), Vertice::listVertices.end(), [&window] (const Vertice & v) {
				sf::CircleShape circle;
				circle.setPosition(v.x-1,v.y-1);
				circle.setRadius(2);
				circle.setFillColor(sf::Color::White);
				window->draw(circle);
	});

	std::for_each(Button::buttonList.begin(),Button::buttonList.end(), [&] (Button * b) {
		window->draw(*b);
	});

	window->display();
	window->clear(sf::Color::Black);
}

void StateTBB::KeyPressed(sf::Event::KeyEvent keyEvent) {
	if (keyEvent.code == sf::Keyboard::Escape)
		StateManager::SetActiveState(STATE_NONE);
	if (keyEvent.code == sf::Keyboard::F1)
		StateManager::SetActiveState(STATE_GUI);
	if (keyEvent.code == sf::Keyboard::F2)
		StateManager::SetActiveState(STATE_THREAD);
}
void StateTBB::MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent) {
}
void StateTBB::MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent) {
}
void StateTBB::MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent) {
}
void StateTBB::MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent) {
}
