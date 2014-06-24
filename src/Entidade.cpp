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
void Entidade::Unload() {
	this->m_vertices.clear();
	//sf::Texture nTexture;
	this->m_texture = sf::Texture();

}
