/*
 * Entidade.h
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#ifndef ENTIDADE_H_
#define ENTIDADE_H_

#include <vector>
#include <SFML/Graphics.hpp>
#include "FPS.h"
#include "Resources.h"

class Entidade : public sf::Drawable, public sf::Transformable {
public:
	static std::vector< Entidade *> entityList;
	Entidade();
	Entidade(sf::Vector2f size, sf::Vector2f position);
	virtual void OnLoop();
	virtual void Unload();
	sf::VertexArray & getShape();
	bool Collided(sf::FloatRect foreignBox);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_texture;

};

class Button : public sf::Drawable, public sf::Transformable {
public:
	static std::vector< Button *> buttonList;
	Button(sf::Text text);
	sf::Text & getText();
	sf::RectangleShape & getShape();

	virtual void OnLoop();
	virtual void Unload();

	bool Collided(sf::Vector2f points);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::FloatRect boundingBox;
	sf::Text m_Text;
	sf::RectangleShape m_Shape;
};

#endif /* ENTIDADE_H_ */
