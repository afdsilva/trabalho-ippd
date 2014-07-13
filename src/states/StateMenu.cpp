/*
 * StateMenu.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "StateMenu.h"
#include "../gui/StateManager.h"

StateMenu StateMenu::instance;

StateMenu::StateMenu() {
	m_Mesh = new Graph(*(new Triangle()));
	nThreadsText = NULL;
	nTrianglesText = NULL;
	nTimeText = NULL;
	nListSize = NULL;
	n_Threads = 1;
}
StateMenu * StateMenu::GetIntance() {
	return & instance;
}

void StateMenu::OnActivation() {
	ResourceFonts::loadFonts();
	n_Threads = 1;
	nThreadsText = new Button(sf::Text("Threads: ",ResourceFonts::getFontResource(FONTS::ScienceProject),24));
	nThreadsText->getText().setColor(sf::Color::White);

	Entidade::entityList.clear();
	draw_Triangles.clear();

//	std::vector<std::thread> threads;
//	Delaunay d(m_Triangles,*m_Mesh,mutex);
//
//	itVertices begin,end;
//	int size = m_Vertices.size();
//	for(int i = 0 ;i < n_Threads; i++) {
//		begin = m_Vertices.begin() + (i * size / n_Threads);
//		end = m_Vertices.begin() + ((i+1) * size / n_Threads);
//		end = ((i+1) * size / n_Threads) > size ? m_Vertices.end() : end;
//		threads.push_back(std::thread(d,begin,end));
//	}
//	for(auto& thread : threads){
//		thread.join();
//	}

    std::string trText = "Entrada: " + std::to_string(m_Vertices.size()) + " Saida: " + std::to_string(m_Triangles.size());
	nTrianglesText = new Button(sf::Text(trText, ResourceFonts::getFontResource(FONTS::ScienceProject),24));
	nTrianglesText->getText().setColor(sf::Color::White);
	nTrianglesText->move(200,0);
	nTimeText = new Button(sf::Text("Tempo: ", ResourceFonts::getFontResource(FONTS::ScienceProject),24));
	nTimeText->getText().setColor(sf::Color::White);
	nTimeText->move(600,0);
	nListSize = new Button(sf::Text("Lista: ", ResourceFonts::getFontResource(FONTS::ScienceProject),24));
	nListSize->getText().setColor(sf::Color::White);
	nListSize->move(0,50);

}
void StateMenu::OnDeactivation() {
	Entidade::entityList.clear();
	for(auto& thread : threads){
		if (thread.joinable())
			thread.join();
	}
}
void StateMenu::OnLoop() {
	std::string text = "Threads: ";
	text +=std::to_string(n_Threads);

	nThreadsText->getText().setString(text);
	if (!m_VerticeList.empty()) {
		std::string textl = "Lista: " + std::to_string(m_VerticeList.size());
		nListSize->getText().setString(textl);
	}
}
void StateMenu::OnRender(sf::RenderWindow * window) {
	mutex.lock();
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
	mutex.unlock();

	std::for_each(draw_Triangles.begin(), draw_Triangles.end(), [&window](Triangle * t) {
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
	});
	std::for_each(m_Vertices.begin(), m_Vertices.end(), [&window] (const Vertice & v) {
				sf::CircleShape circle;
				circle.setPosition(v.x-1,v.y-1);
				circle.setRadius(2);
				circle.setFillColor(sf::Color::White);
				window->draw(circle);
	});
	window->draw(*nThreadsText);
	window->draw(*nTrianglesText);
	window->draw(*nTimeText);
	window->draw(*nListSize);

	window->display();
	window->clear(sf::Color::Black);
}

void StateMenu::KeyPressed(sf::Event::KeyEvent keyEvent) {
	if (keyEvent.code == sf::Keyboard::Escape)
		StateManager::SetActiveState(STATE_NONE);
	if (keyEvent.code == sf::Keyboard::D)
		cavity = cavity ? false : true;
	if (keyEvent.code == sf::Keyboard::E)
		m_Triangles = m_Mesh->updateList();
	if (keyEvent.code == sf::Keyboard::R) {
		GenerateVertices v(1000,m_Vertices);
	}
	if (keyEvent.code == sf::Keyboard::T) {
		GenerateVertices v("dados.txt",m_Vertices);
	}
	if (keyEvent.code == sf::Keyboard::Q) {
		m_VerticeList = std::list<Vertice>(m_Vertices.begin(),m_Vertices.end());
	}
	if (keyEvent.code == sf::Keyboard::Add || keyEvent.code == sf::Keyboard::Equal)
		n_Threads++;
	if (keyEvent.code == sf::Keyboard::Subtract || keyEvent.code == sf::Keyboard::Dash)
		n_Threads = n_Threads > 1 ? n_Threads-1 : n_Threads;

	if (keyEvent.code == sf::Keyboard::Return) {
		if (m_VerticeList.size() > 3) {

			m_Mesh = new Graph(Triangle::Triangulation(m_Vertices));

			m_Triangles.clear();
			m_Triangles.push_back(&m_Mesh->getDescritor());
			threads.clear();
			clock.restart();
			DelaunayConsumer dc(m_VerticeList,m_Triangles,*m_Mesh,mutex);
			sfThreads.clear();
			for (int i = 0; i < n_Threads; i++) {
				sf::Thread * t = new sf::Thread(dc,i);
				sfThreads.push_back(t);
//				threads.push_back(std::thread(dc,i));
			}
			for (int i = 0; i < n_Threads; i++) {
				sfThreads[i]->launch();
//				threads.push_back(std::thread(dc,i));
			}
//			for(auto& thread : threads){
//					thread.join();
//			}
			sf::Time elapsed1 = clock.getElapsedTime();

			std::string trText = "Entrada: " + std::to_string(m_Vertices.size()) + " Saida: " + std::to_string(m_Triangles.size());
			nTrianglesText->getText().setString(trText);
			std::string tiText = "Tempo: " + std::to_string(elapsed1.asSeconds());
			nTimeText->getText().setString(tiText);
			//std::cout << "Tempo de algoritmo: " << elapsed1.asSeconds() << " Entrada: " << m_Vertices.size() << " Saida: " << m_Triangles.size() << std::endl;
			clock.restart();
		}
	}

}
void StateMenu::MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent) {
	draw_Triangles.clear();
	if (cavity) {
		Triangle * search = NULL;
		Vertice p((float)mouseMoveEvent.x, (float)mouseMoveEvent.y);
		for_each(m_Triangles.begin(), m_Triangles.end(),[&](Triangle * t) {
			if (t->containsVertex(p)) {
				search = t;
			}
		});
		if (search) {
			Cavity * cavity = new Cavity(*search,p);
			cavity->expand();
			cavity->retriangulate();
			draw_Triangles = cavity->getNewTriangles();
		}
	}
}
void StateMenu::MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent) {
	if (mouseButtonEvent.button == sf::Mouse::Right) {
		mouseRight = true;
	}
	if (mouseButtonEvent.button == sf::Mouse::Left)
		mouseLeft = true;
}
void StateMenu::MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent) {
	if (mouseButtonEvent.button == sf::Mouse::Right)
		mouseRight = false;
	if (mouseButtonEvent.button == sf::Mouse::Left)
		mouseLeft = false;
}
void StateMenu::MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent) {
}
