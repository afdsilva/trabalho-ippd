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
	void operator()(const Edge &e) const {
		//sf::Window

		window.draw(e);
	}
protected:

	sf::RenderWindow & window;
};


#endif /* ENTIDADE_H_ */
