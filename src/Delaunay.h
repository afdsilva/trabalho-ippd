#ifndef DELAUNAY_H_
#define DELAUNAY_H_

#include <iostream>
#include <set>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Geometry.h"
#include "Quadtree.h"
#include <thread>
#include <future>

using namespace std;

typedef std::set<Vertex *> vertexSet;
typedef std::set<Vertex *>::iterator vIterator;
typedef std::set<Vertex *>::const_iterator cvIterator;

typedef std::vector<Triangle> triangleSet;
typedef std::vector<Triangle>::iterator tIterator;
typedef std::vector<Triangle>::const_iterator ctIterator;

typedef std::set<Edge> edgeSet;
typedef std::set<Edge>::iterator eIterator;
typedef std::set<Edge>::const_iterator ceIterator;

class Delaunay {
public:
	static void Triangulate(const vertexSet & vertices, triangleSet & output);
};

#endif /* DELAUNAY_H_ */
