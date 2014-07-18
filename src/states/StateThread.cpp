/*
 * StateThread.cpp
 *
 *  Created on: 16/07/2014
 *      Author: andref
 */

#include "StateThread.h"
#include "../gui/StateManager.h"

StateThread StateThread::instance;

StateThread::StateThread() {
	n_Threads = 4;
	n_Entries = 1000;
	n_Depth = 3;
	quadTree = NULL;
	m_Mesh = NULL;
}
StateThread * StateThread::GetIntance() {
	return & instance;
}
void StateThread::OnActivation() {
	ResourceFonts::loadFonts();
	Entidade::entityList.clear();
	Button::buttonList.clear();

	m_Mesh = &DelaunayThread::ExecuteThread(Vertice::listVertices, App::n_Threads, 3);
	m_Triangles = m_Mesh->getTriangleList();

}

void StateThread::OnDeactivation() {
	Entidade::entityList.clear();
}
void StateThread::OnLoop() {

}
void StateThread::OnRender(sf::RenderWindow * window) {
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

void StateThread::KeyPressed(sf::Event::KeyEvent keyEvent) {
	if (keyEvent.code == sf::Keyboard::Escape)
		StateManager::SetActiveState(STATE_NONE);
	if (keyEvent.code == sf::Keyboard::F1)
		StateManager::SetActiveState(STATE_GUI);
	if (keyEvent.code == sf::Keyboard::F3)
		StateManager::SetActiveState(STATE_TBB);
}
void StateThread::MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent) {
}
void StateThread::MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent) {
}
void StateThread::MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent) {
}
void StateThread::MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent) {
}
