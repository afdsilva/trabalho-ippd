/*
 * Entidade.cpp
 *
 *  Created on: 23/06/2014
 *      Author: andref
 */

#include "Entidade.h"

std::vector<Entidade *> Entidade::entityList;
Entidade::Entidade() {

}
Entidade::Entidade(sf::Vector2f size, sf::Vector2f position) {
	m_vertices = sf::VertexArray(sf::Quads,4);
	m_vertices[0].position = sf::Vector2f(0,0);
	m_vertices[1].position = sf::Vector2f(size.x, 0);
	m_vertices[2].position = sf::Vector2f(size.x, size.y);
	m_vertices[3].position = sf::Vector2f(0, size.y);
}
void Entidade::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// apply the entity's transform -- combine it with the one that was passed by the caller
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

	// apply the texture
	states.texture = &m_texture;

	// draw the vertex array
	target.draw(m_vertices, states);
}
sf::VertexArray & Entidade::getShape() {
	return m_vertices;
}
void Entidade::OnLoop() {

}
bool Entidade::Collided(sf::FloatRect foreignBox) {
	sf::FloatRect boundingBox = this->m_vertices.getBounds();

	if (boundingBox.intersects(foreignBox)) {
		return true;
	}
	return false;
}
void Entidade::Unload() {
	this->m_vertices.clear();
	//sf::Texture nTexture;
	this->m_texture = sf::Texture();

}

Button::Button(sf::Text text) {
	m_Text = text;
	sf::FloatRect boundingBox = m_Text.getGlobalBounds();
	m_Shape.setPosition(boundingBox.left,boundingBox.top);
	m_Shape.setSize(sf::Vector2f(boundingBox.width,boundingBox.height));
	m_Shape.setFillColor(sf::Color::Transparent);

}
bool Button::Collided(sf::Vector2f points) {
	sf::FloatRect boundingBox = this->m_Shape.getGlobalBounds();
	if (boundingBox.contains(points)) {
		return true;
	}
	return false;
}
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(m_Shape, states);
	target.draw(m_Text, states);
}

sf::Text & Button::getText() {
	return m_Text;
}
sf::RectangleShape & Button::getShape() {
	return m_Shape;
}
void Button::OnLoop() {

}
void Button::Unload() {

}
