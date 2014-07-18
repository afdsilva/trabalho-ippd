/*
 * DelaunayTBB.h
 *
 *  Created on: 15/07/2014
 *      Author: andref
 */

#ifndef DELAUNAYTBB_H_
#define DELAUNAYTBB_H_

#include <SFML/System.hpp>

#include <tbb/tbb.h>
#include <tbb/mutex.h>
#include <tbb/parallel_for.h>
#include "../Graph.h"
#include "../Quadtree.h"

class CavityTBB {
public:
	CavityTBB(Triangle & t,const Vertice & v) : m_Triangle(t), m_Vertice(v), m_EdgesList(new EdgeList),  m_EdgesLock(new EdgeList), m_TrianglesCavity(new TriangleList), m_TrianglesLock(new TriangleList), m_TriangleSet(new TriangleList){}
	bool expand();
	void retriangulate();
	TriangleList & getNewTriangles() const;
	TriangleList & getModifiedTriangles() const;
	bool lockEdges(tbb::mutex & mutex);
	bool lockTriangles(tbb::mutex & mutex);
	void unlockEdges();
	void unlockExpandEdges();
	void unlockTriangles();
protected:
	bool expand(Triangle & t);

	Triangle & m_Triangle;
	Vertice m_Vertice;
	EdgeList * m_EdgesList;
	EdgeList * m_EdgesLock;
	TriangleList * m_TrianglesCavity;
	TriangleList * m_TrianglesLock;
	TriangleList * m_TriangleSet;

};

class GraphTBB {
public:
	GraphTBB(Triangle & descritor,Quadtree & quad) : m_Descritor(&descritor), m_Quadtree(quad), m_TrianglesList(new TriangleList) {
		m_Quadtree.AddObject(&descritor);
	}
	void updateGraph(CavityTBB & c);
	void updateList();
	Triangle & getDescritor() const;
	TriangleList & getTriangleList() const;
	Triangle * searchTriangle(const Vertice & v);
protected:
	void updateList(Triangle & t);
	Triangle * m_Descritor;
	Quadtree & m_Quadtree;
	TriangleList * m_TrianglesList;
};

class DelaunayTBB {
public:
	DelaunayTBB(VerticeList & vertices, GraphTBB & mesh, tbb::mutex & mutex) : m_Vertices(vertices), m_Mesh(mesh), m_Mutex(mutex) {};
	virtual ~DelaunayTBB() {}
	void operator()( const tbb::blocked_range<size_t>& r ) const;
	static TriangleList & ExecuteTBB(VerticeList & vertices, int threads);
protected:
	VerticeList & m_Vertices;
	GraphTBB & m_Mesh;
	tbb::mutex & m_Mutex;
};

#endif /* DELAUNAYTBB_H_ */
