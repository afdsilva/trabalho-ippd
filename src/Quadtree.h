#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>
#include "Geometry.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace std;

class Quadtree;

class Quadtree {
public:
	Quadtree();
	Quadtree(float x, float y, float width, float height, int level, int maxLevel);
						
						~Quadtree();

	void					AddObject(Vertex *object);
	vector<Vertex*>				GetObjectsAt(float x, float y);
	void					Clear();

	void					SetFont(const sf::Font &font);
	void					Draw(sf::RenderTarget &canvas);

private:
	float					x;
	float					y;
	float					width;
	float					height;
	int					level;
	int					maxLevel;
	vector<Vertex*>				objects;

	Quadtree *				parent;
	Quadtree *				NW;
	Quadtree *				NE;
	Quadtree *				SW;
	Quadtree *				SE;

	sf::RectangleShape			shape;
	sf::Text				text;

	bool					contains(Quadtree *child, Vertex *object);
};

#endif
