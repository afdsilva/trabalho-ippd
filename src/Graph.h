/*
 * Graph.h
 *
 *  Created on: 30/06/2014
 *      Author: andref
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <exception>
#include <iostream>
#include <mutex>
#include <sstream>

class Edge;
class Vertice;
class Triangle;
class GenerateVertices;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

typedef std::vector<Edge *> EdgeList;
typedef std::vector<Edge *>::iterator itEdges;
typedef std::vector<Triangle *> TriangleList;
typedef std::vector<Triangle *>::iterator itTriangles;
typedef std::vector<Vertice> VerticeList;
typedef std::vector<Vertice>::iterator itVertices;


class Vertice {
public:
	Vertice() : x(.0f), y(.0f) {}
	Vertice(float _x, float _y) : x(_x), y(_y) {}
	float x;
	float y;
	bool operator<(const Vertice& v) const {
		if (x == v.x) return y < v.y;
		return x < v.x;
	}
	bool operator==(const Vertice& v) const {
		return (x == v.x && y == v.y);
	}
	Vertice operator-(const Vertice & b) const {
		return Vertice(b.x - x, b.y - y);
	}
	friend std::ostream& operator<<(std::ostream& os, const Vertice& obj) {
		os << "x = " << obj.x << " y = " << obj.y;
		return os;
	}
	static VerticeList listVertices;
	bool cowjuggling = false;
};

class Edge {
public:
	Edge(Vertice _v0, Vertice _v1);
	void setAdjancency(Triangle & t);
	void setAdjancency(Triangle & t, int n);
	void remAdjancency(Triangle & t);
	void remAdjancency(int n);

	Triangle * getT1() const {
		return m_Triangles[0];
	}
	Triangle * getT2() const {
		return m_Triangles[1];
	}
	Triangle * getTriangles() const {
		return *m_Triangles;
	}
	Vertice getV0() const {
		return m_Vertices[0];
	}
	Vertice getV1() const {
		return m_Vertices[1];
	}
	Vertice * getVertices() {
		return m_Vertices;
	}
	static Vertice CommonVertice(Edge & e1, Edge & e2);
	bool hasCommomVertice(Edge & e);
	bool operator==(const Edge& e) const {
		if (getV0() == e.getV0() && getV1() == e.getV1())
			return true;
		return (getV0() == e.getV1() && getV1() == e.getV0());
	}

	friend std::ostream& operator<<(std::ostream& os, const Edge& obj) {
		os << "Vertice 0 = " << obj.getV0() << " Vertice 1 = " << obj.getV1();
		return os;
	}
	//cada edge tem seu mutex particular
	std::mutex e_Mutex; //mutex publico pq qualquer um vai precisar acessar essa porquera (e pq n to afim de fazer getter
	std::mutex r_Mutex; //mutex utilizado para remover a referencia da edge
protected:
	Vertice m_Vertices[2];
	Triangle * m_Triangles[2];

};

class Triangle {
public:
	Triangle() : e0(NULL), e1(NULL), e2(NULL), m_R(0), m_R2(0) {}
	Triangle(Vertice & a, Vertice & b, Vertice & c);
	Triangle(Edge & e, Vertice & a);
	Triangle(Edge & _e0, Edge & _e1, Edge & _e2);
	virtual ~Triangle();
	void setCircumCircle();
	bool containsEdge(const Edge & e) const;
	bool containsVertex(const Vertice & v) const;
	Vertice baryCoords(const Vertice & p) const;
	Vertice getCentroid() const;
	bool containsTriangle(const Triangle & t) const;
	friend std::ostream& operator<<(std::ostream& os, const Triangle& obj) {
		if (&obj != NULL)
			os << "Triangle: " << std::endl << "Edge 0: " << obj.getE0() << std::endl << "Edge 1: " << obj.getE1() << std::endl << " Edge 2: " << obj.getE2();
		return os;
	}
	bool CCEncompasses(const Vertice & v) const;
	Edge & getE0() const {
		return *e0;
	}
	Edge & getE1() const {
		return *e1;
	}
	Edge & getE2() const {
		return *e2;
	}
	Vertice getCenter() const {
		return m_Center;
	}
	float getR() const {
		return m_R;
	}
	float getR2() const {
		return m_R2;
	}
	static Triangle & Triangulation(std::vector<Vertice> & m_Vertices);

	std::mutex t_Mutex; //usado na expansao
	std::mutex r_Mutex; //usado na procura
	std::mutex n_Mutex; //usado na atualizacao da quadtree (previne recursao)
protected:
	Edge * e0;
	Edge * e1;
	Edge * e2;
	Vertice m_Center;
	Vertice m_Centroid;
	float m_R;
	float m_R2;


};

class GenerateVertices {
public:
	//Gerenate random vertices
	GenerateVertices(int n, VerticeList & vertices) : m_Vertices(vertices) {
		float resX = 1024;
		float resY = 768;
		int boxSizeX = resX / 1.5;
		int boxSizeY = resY / 1.5;
		m_Vertices.clear();
		for(int i = 0; i < n; i++) {
			float x = (resX / 2) - boxSizeX / 2 + (rand() % boxSizeX);
			float y = (resY / 2) - boxSizeY / 2 + (rand() % boxSizeY);
			Vertice p(x,y);
			m_Vertices.push_back(p);
		}
	}
	GenerateVertices(std::string file, VerticeList & vertices) : m_Vertices(vertices) {
		std::ifstream myfile;
		m_Vertices.clear();
		myfile.open (file);
		if (myfile.is_open()) {
			std::string line;
			std::vector<std::string> ssplit;
			int i = 0;
			while ( getline (myfile,line) ) {
				ssplit = split(line,' ');
				if (ssplit.size() > 0) {
					int x = atoi(ssplit[0].c_str());
					int y = !ssplit[1].empty() ? atoi(ssplit[1].c_str()) : atoi(ssplit[2].c_str());
					if (y == 0) std::cout << line << std::endl;
					Vertice p(x,y);
					m_Vertices.push_back(p);
					i++;
				}
			}

			myfile.close();
		}
		else std::cout << "Unable to open file";

	}
protected:
	VerticeList & m_Vertices;
};

#endif /* GRAPH_H_ */
