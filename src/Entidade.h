/*
 * Entidade.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef ENTIDADE_H_
#define ENTIDADE_H_

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "FPS.h"
#include "Delaunay.h"

class Entidade : public sf::Drawable, public sf::Transformable  {
public:
	static std::vector< Entidade *> entityList;
	Entidade();

	virtual void OnLoop();
	virtual void Unload();
	sf::VertexArray & getShape();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_texture;

};

class Linha {
public:
	Linha(sf::RenderWindow & w) : window(w) {}
	void operator()(const edge &e) const {
		//sf::Window
		sf::Vertex m_line[2];
		m_line[0] = sf::Vector2f(e.m_pV0->GetX(), e.m_pV0->GetY());
		m_line[1] = sf::Vector2f(e.m_pV1->GetX(), e.m_pV1->GetY());
		window.draw(m_line,2,sf::Lines);
	}
protected:

	sf::RenderWindow & window;
};

class Triangulo {
public:
	Triangulo(sf::RenderWindow & w) : window(w) {}
	void operator()(const triangle &t) const {
		const vertex * v0 = t.GetVertex(0);
		const vertex * v1 = t.GetVertex(1);
		const vertex * v2 = t.GetVertex(2);
		sf::VertexArray triangle(sf::Triangles, 3);

		// define the position of the triangle's points
		triangle[0].position = sf::Vector2f(v0->GetX(), v0->GetY());
		triangle[1].position = sf::Vector2f(v1->GetX(), v1->GetY());
		triangle[2].position = sf::Vector2f(v2->GetX(), v2->GetY());

		window.draw(triangle);
	}
protected:

	sf::RenderWindow & window;
};

class Circulo {
public:
	Circulo(sf::RenderWindow & w) : window(w) {}
	void operator()(const vertex &v) const {
		sf::CircleShape m_circle;
		m_circle.setPosition(sf::Vector2f(v.GetX()-1,v.GetY()-1));
		m_circle.setRadius(2);
		window.draw(m_circle);
	}
protected:

	sf::RenderWindow & window;
};



#endif /* ENTIDADE_H_ */
