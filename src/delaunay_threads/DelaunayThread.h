/*
 * DelaunayThread.h
 *
 *  Created on: 13/07/2014
 *      Author: andref
 */

#ifndef DELAUNAYTHREAD_H_
#define DELAUNAYTHREAD_H_

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <list>
#include <SFML/System.hpp>

#include "../Quadtree.h"
#include "../Graph.h"

class Cavity {
public:
	Cavity(Triangle & t,const Vertice & v) : m_Triangle(t), m_Vertice(v), m_EdgesList(new EdgeList),  m_EdgesLock(new EdgeList), m_TrianglesCavity(new TriangleList), m_TrianglesLock(new TriangleList), m_TriangleSet(new TriangleList){}
	bool expand();
	bool expandLock();
	void retriangulate();
	TriangleList & getNewTriangles() const;
	TriangleList & getModifiedTriangles() const;
	bool lockEdges(std::mutex & mutex);
	bool lockTriangles(std::mutex & mutex);
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

class Graph {
public:
	Graph(Triangle & descritor,Quadtree & quad) : m_Descritor(&descritor), m_Quadtree(quad), m_TrianglesList(new TriangleList) {}
	void updateGraph(Cavity & c, std::mutex & mutex);
	void updateList();
	Triangle & getDescritor() const;
	TriangleList & getTriangleList() const;
protected:
	void updateList(Triangle & t);
	Triangle * m_Descritor;
	Quadtree & m_Quadtree;
	TriangleList * m_TrianglesList;
};

class DelaunayThread {
public:
	DelaunayThread(std::list<Vertice> & list, Quadtree & quadtree, Graph & mesh, std::mutex & mutex, sf::Time & etime) : m_List(list), m_Quadtree(quadtree), m_Mesh(mesh), m_Mutex(mutex), m_ElapsedTime(etime) {}
	void operator()(int nThread);
	static Graph & ExecuteThread(VerticeList & vertices, int threads, int depth);
protected:
	Triangle * searchTriangle(const Vertice & v);
	std::list<Vertice> & m_List;
	Quadtree & m_Quadtree;
	Graph & m_Mesh;
	std::mutex & m_Mutex;
	sf::Time & m_ElapsedTime;

};

#endif /* DELAUNAYTHREAD_H_ */
