/*
 * StateGUI.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "StateGUI.h"
#include "../gui/StateManager.h"

StateGUI StateGUI::instance;

StateGUI::StateGUI() {
	m_Mesh = NULL;
	m_Window = NULL;
	nThreadsText = NULL;
	quadTree = NULL;
	nTrianglesText = NULL;
	nTimeText = NULL;
	nThreadsPlus = NULL;
	nThreadsMinus = NULL;
	nListSize = NULL;
	n_Depth = 4;
}
StateGUI * StateGUI::GetIntance() {
	return & instance;
}

void StateGUI::OnActivation() {
	Entidade::entityList.clear();
	draw_Triangles.clear();
	Button::buttonList.clear();
	ResourceFonts::loadFonts();


	/**
	 * Buttons
	 */
	nThreadsText = new Button(sf::Text("Threads: ",ResourceFonts::getFontResource(FONTS::ScienceProject),24));
	Button::buttonList.push_back(nThreadsText);
	nThreadsPlus = new Button(sf::Text("+",ResourceFonts::getFontResource(FONTS::Serious1),24));
	Button::buttonList.push_back(nThreadsPlus);
	nThreadsMinus = new Button(sf::Text("-",ResourceFonts::getFontResource(FONTS::Serious1),24));
	Button::buttonList.push_back(nThreadsMinus);

    std::string trText = "Entrada: " + std::to_string(Vertice::listVertices.size()) + " Saida: " + std::to_string(m_Triangles.size());
	nTrianglesText = new Button(sf::Text(trText, ResourceFonts::getFontResource(FONTS::ScienceProject),24));
	nTrianglesText->move(200,0);
	Button::buttonList.push_back(nTrianglesText);
	nTimeText = new Button(sf::Text("Tempo: ", ResourceFonts::getFontResource(FONTS::ScienceProject),24));
	nTimeText->move(600,0);
	Button::buttonList.push_back(nTimeText);
	nListSize = new Button(sf::Text("Lista: ", ResourceFonts::getFontResource(FONTS::ScienceProject),24));
	nListSize->move(0,50);
	Button::buttonList.push_back(nListSize);
	std::for_each(Button::buttonList.begin(),Button::buttonList.end(), [] (Button * b) {
		b->getText().setColor(sf::Color::White);
	});
	GenerateVertices v(App::n_Entries,Vertice::listVertices);
	m_VerticeList = std::list<Vertice>(Vertice::listVertices.begin(),Vertice::listVertices.end());

	FPS::FPSControl.maxFrame = 60;

}
void StateGUI::OnDeactivation() {
	Entidade::entityList.clear();
}
void StateGUI::OnLoop() {
	if (m_Mesh) {
		mutex.lock();
		m_Triangles = m_Mesh->getTriangleList();
		mutex.unlock();
	}
	std::string text = "Threads: " + std::to_string(App::n_Threads);
	nThreadsText->getText().setString(text);
	std::string textl = "Lista: " + std::to_string(m_VerticeList.size());
	nListSize->getText().setString(textl);
	std::string trText = "Entrada: " + std::to_string(App::n_Entries) + " Saida: " + std::to_string(m_Triangles.size());
	nTrianglesText->getText().setString(trText);
	std::string tiText = "Tempo: " + std::to_string(totalElapsed.asSeconds());
	nTimeText->getText().setString(tiText);
	nThreadsPlus->setPosition(nThreadsText->getText().getLocalBounds().width + 5,0);
	nThreadsMinus->setPosition(nThreadsText->getText().getLocalBounds().width + 5,20);
//	quadTree->Clear();

//	std::for_each(m_Triangles.begin(), m_Triangles.end(), [&](Triangle * t) {
//		quadTree->AddObject(t);
//	});
	std::for_each(Button::buttonList.begin(),Button::buttonList.end(), [&] (Button * b) {
		b->OnLoop();
	});


}
void StateGUI::OnRender(sf::RenderWindow * window) {
	m_Window = window;
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
		if (t) {
			sf::VertexArray shape(sf::Triangles,3);
			Vertice a = Edge::CommonVertice(t->getE2(),t->getE0()); //CA AB
			Vertice b = Edge::CommonVertice(t->getE0(),t->getE1()); //AB BC
			Vertice c = Edge::CommonVertice(t->getE1(),t->getE2()); //BC CA

			shape[0] = sf::Vertex(sf::Vector2f(a.x,a.y));
			shape[0].color = sf::Color::Magenta;
			shape[1] = sf::Vertex(sf::Vector2f(b.x,b.y));
			shape[1].color = sf::Color::Yellow;
			shape[2] = sf::Vertex(sf::Vector2f(c.x,c.y));
			shape[2].color = sf::Color::Cyan;
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

//	quadTree->Draw(*window);

	std::for_each(Button::buttonList.begin(),Button::buttonList.end(), [&] (Button * b) {
		window->draw(*b);
	});

	window->display();
	window->clear(sf::Color::Black);
}

void StateGUI::KeyPressed(sf::Event::KeyEvent keyEvent) {
	if (keyEvent.code == sf::Keyboard::Escape)
		StateManager::SetActiveState(STATE_NONE);
	if (keyEvent.code == sf::Keyboard::F2)
		StateManager::SetActiveState(STATE_THREAD);
	if (keyEvent.code == sf::Keyboard::F3)
		StateManager::SetActiveState(STATE_TBB);



	if (keyEvent.code == sf::Keyboard::D)
		cavity = cavity ? false : true;

	if (keyEvent.code == sf::Keyboard::LBracket) {
		App::n_Entries+=10;
	}
	if (keyEvent.code == sf::Keyboard::RBracket) {
		App::n_Entries -= (App::n_Entries > 10 ? 10 : 0);
	}
	if (keyEvent.code == sf::Keyboard::R) {
		GenerateVertices v(App::n_Entries,Vertice::listVertices);
	}
	if (keyEvent.code == sf::Keyboard::T) {
		GenerateVertices v("dados.txt",Vertice::listVertices);
	}

	if (keyEvent.code == sf::Keyboard::Add || keyEvent.code == sf::Keyboard::Equal)
		App::n_Threads++;
	if (keyEvent.code == sf::Keyboard::Subtract || keyEvent.code == sf::Keyboard::Dash)
		App::n_Threads = App::n_Threads > 1 ? App::n_Threads-1 : App::n_Threads;

	if (keyEvent.code == sf::Keyboard::Return) {
		if (App::n_Entries != (int) Vertice::listVertices.size())
			GenerateVertices v(App::n_Entries,Vertice::listVertices);
		m_VerticeList = std::list<Vertice>(Vertice::listVertices.begin(),Vertice::listVertices.end());
		if (m_VerticeList.size() > 2) {
			Triangle * descritor = &Triangle::Triangulation(Vertice::listVertices);

			quadTree = new Quadtree(0.0f,0.0f,1024.0f,768.0f,0,n_Depth);
			quadTree->AddObject(descritor);
			m_Mesh = new Graph(*descritor, *quadTree);

			m_Triangles.clear();
			m_Triangles.push_back(&m_Mesh->getDescritor());
			threads.clear();
			sfThreads.clear();
			clock.restart();
			totalElapsed = sf::Time();
			for (int i = 0; i < App::n_Threads; i++) {
				DelaunayThread dt(m_VerticeList,*quadTree,*m_Mesh,mutex,totalElapsed);
				sf::Thread * thread = new sf::Thread(dt,i);
				sfThreads.push_back(thread);
			}
			for (int i = 0; i < App::n_Threads; i++) {
				sfThreads[i]->launch();
			}
		}
	}

}
void StateGUI::MouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent) {
	draw_Triangles.clear();
	TriangleList searchQuadTree;
	if (quadTree)
		searchQuadTree = quadTree->GetObjectsAt((float)mouseMoveEvent.x, (float)mouseMoveEvent.y);

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
			if (cavity->expand()) {
				cavity->retriangulate();
				draw_Triangles = cavity->getNewTriangles();
			}
			cavity->unlockEdges();
			cavity->unlockTriangles();
		}
	} else {
		draw_Triangles = searchQuadTree;
	}
	if (nThreadsPlus->Collided(sf::Vector2f(mouseMoveEvent.x, mouseMoveEvent.y))) {
		nThreadsPlus->getShape().setOutlineThickness(1.0f);
		nThreadsPlus->getShape().setOutlineColor(sf::Color::White);
	} else {
		nThreadsPlus->getShape().setOutlineColor(sf::Color::Transparent);
	}
	if (nThreadsMinus->Collided(sf::Vector2f(mouseMoveEvent.x, mouseMoveEvent.y))) {
		nThreadsMinus->getShape().setOutlineThickness(1.0f);
		nThreadsMinus->getShape().setOutlineColor(sf::Color::White);
	} else {
		nThreadsMinus->getShape().setOutlineColor(sf::Color::Transparent);
	}
}
void StateGUI::MouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent) {
	if (mouseButtonEvent.button == sf::Mouse::Left && nThreadsPlus->Collided(sf::Vector2f(mouseButtonEvent.x, mouseButtonEvent.y))) {
		App::n_Threads++;
	}
	if (mouseButtonEvent.button == sf::Mouse::Left && nThreadsMinus->Collided(sf::Vector2f(mouseButtonEvent.x, mouseButtonEvent.y))) {
		App::n_Threads = App::n_Threads > 1 ? App::n_Threads-1 : App::n_Threads;
	}

	if (mouseButtonEvent.button == sf::Mouse::Right) {
		mouseRight = true;
	}
	if (mouseButtonEvent.button == sf::Mouse::Left)
		mouseLeft = true;
}
void StateGUI::MouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent) {
	if (mouseButtonEvent.button == sf::Mouse::Right)
		mouseRight = false;
	if (mouseButtonEvent.button == sf::Mouse::Left)
		mouseLeft = false;
}
void StateGUI::MouseWheelMoved(sf::Event::MouseWheelEvent mouseWheelEvent) {
}
