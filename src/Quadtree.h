#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Graph.h"

using namespace std;

class Quadtree;
class Graph;

class Quadtree {
public:
						Quadtree(float x, float y, float width, float height, int level, int maxLevel);
						
						~Quadtree();

	void					AddObject(Triangle *object);
	vector<Triangle*>		GetObjectsAt(float x, float y);
	vector<Triangle*>		GetObjects();
	int						GetQuadrantSize(float _x, float _y);
	int						GetTreeSize();
	void					Clear();
	bool					Inserted(Triangle * object);

	void					SetFont(const sf::Font &font);
	void					Draw(sf::RenderTarget &canvas);

private:
	float					x;
	float					y;
	float					width;
	float					height;
	int					level;
	int					maxLevel;
	vector<Triangle*>				objects;

	Quadtree *				parent;
	Quadtree *				NW;
	Quadtree *				NE;
	Quadtree *				SW;
	Quadtree *				SE;

	sf::RectangleShape			shape;
	sf::Text				text;

	bool					contains(Quadtree *child, Triangle *object);
};

#endif
