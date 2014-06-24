/*
 * StateMenu.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "StateMenu.h"
#include "StateManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

StateMenu StateMenu::instance;

StateMenu::StateMenu() {
}

void StateMenu::OnActivation() {
	Entidade::entityList.clear();

	m_Vertices.clear();
	m_Triangles.clear();
	view.setCenter(sf::Vector2f(2000 / 2 - 1024 / 2, 2000 / 2 - 768 / 2));
	view.setSize(sf::Vector2f(1024,768));
	//view.setViewport(sf::FloatRect(1024, 768, 2000 / 2 - 1024 / 2, 2000 / 2 - 768 / 2));
	/**
	ifstream myfile;
	myfile.open ("dados.txt");
	if (myfile.is_open()) {
		string line;
		vector<string> ssplit;
		while ( getline (myfile,line) ) {
			ssplit = split(line,' ');
			if (ssplit.size() > 0) {
				int x = atoi(ssplit[0].c_str());
				int y = !ssplit[1].empty() ? atoi(ssplit[1].c_str()) : atoi(ssplit[2].c_str());
				if (y == 0) cout << line << endl;
				m_Vertices.insert(vertex(x,y));
			}
			//cout << "X = " << ssplit[0] <<  " Y = " << ssplit[1] << endl;
		}

		myfile.close();
	}
	else cout << "Unable to open file";
	**/

	//gerando vertices aleatorios
	/**/
	for (int i = 0; i < 1000;i++) {
		int x = rand() % 2000;
		int y = rand() % 2000;
		m_Vertices.insert(vertex(x,y));
		//Circulo * mCirculo = new Circulo(sf::Vector2f(x,y),3);
		//Entidade::entityList.push_back(mCirculo);
	}
	/**/
	Delaunay d;
	d.Triangulate(m_Vertices, m_Triangles);
	d.TrianglesToEdges(m_Triangles,m_Edges);

	//Linha * mLinha = new Linha(sf::Vector2f(100,100), sf::Vector2f(200,200));
	//Entidade::entityList.push_back(mLinha);
}
void StateMenu::OnDeactivation() {
	Entidade::entityList.clear();
}
void StateMenu::OnLoop() {
}
void StateMenu::OnRender(sf::RenderWindow * window) {
	/**
	for (unsigned int i = 0; i < Entidade::entityList.size(); i++) {
		Entidade * e = Entidade::entityList[i];
		window->draw(*e);
	}
	**/
	for_each(m_Edges.begin(), m_Edges.end(), Linha(*window));
	//for_each(m_Triangles.begin(), m_Triangles.end(), Triangulo(*window));
	for_each(m_Vertices.begin(), m_Vertices.end(), Circulo(*window));

	window->setView(view);
	window->display();
	window->clear(sf::Color::Black);
}

StateMenu * StateMenu::GetIntance() {
	return & instance;
}
void StateMenu::KeyPressed(sf::Event::KeyEvent keyEvent) {
	if (keyEvent.code == sf::Keyboard::Escape)
		StateManager::SetActiveState(STATE_NONE);
	if (keyEvent.code == sf::Keyboard::Up)
		view.move(0,-5);
	if (keyEvent.code == sf::Keyboard::Down)
		view.move(0,+5);
	if (keyEvent.code == sf::Keyboard::Left)
		view.move(-5,0);
	if (keyEvent.code == sf::Keyboard::Right)
		view.move(+5,0);
	if(keyEvent.code == sf::Keyboard::C)
		view.setCenter(sf::Vector2f(2000 / 2 - 1024 / 2,2000 / 2 - 768 / 2));

}

void StateMenu::MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent) {
	if (mouseButton) {
		float dx = mouseDrag.x - mouseMoveEvent.x;
		float dy = mouseDrag.y - mouseMoveEvent.y;
		view.move(dx,dy);
	}

}

void StateMenu::MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent) {
	if (mouseButtonEvent.button == sf::Mouse::Left) {
		mouseDrag = sf::Vector2f(mouseButtonEvent.x,mouseButtonEvent.y);
		mouseButton = true;
	}
}
void StateMenu::MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent) {
	mouseButton = false;
}
void StateMenu::MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent) {
	if (mouseWheelEvent.delta == 1)
		view.zoom(0.5f);
	if (mouseWheelEvent.delta == -1)
		view.zoom(1.5f);
}
