/*
 * Graph.cpp
 *
 *  Created on: 30/06/2014
 *      Author: andref
 */

#include "Graph.h"

VerticeList Vertice::listVertices;

Triangle::Triangle(Vertice & a, Vertice & b, Vertice & c) {
	 //criar um triangulo pelos vertices, cria de 3 novas edges
	e0 = new Edge(a,b);
	e1 = new Edge(b,c);
	e2 = new Edge(c,a);
	setCircumCircle();
	m_Centroid.x = (a.x + b.x + c.x) / 3;
	m_Centroid.y = (a.y + b.y + c.y) / 3;
}
Triangle::Triangle(Edge & _e0, Edge & _e1, Edge & _e2) {
	//criar um triangulo apartir das edges a funcao pai deve chamar o setAdjancency
	//normalmente as edges jah vao conter triangulos setados
	this->e0 = &_e0;
	this->e1 = &_e1;
	this->e2 = &_e2;
	setCircumCircle();
	Vertice a = Edge::CommonVertice(*this->e2,*this->e0); //CA AB
	Vertice b = Edge::CommonVertice(*this->e0,*this->e1); //AB BC
	Vertice c = Edge::CommonVertice(*this->e1,*this->e2); //BC CA
	m_Centroid.x = (a.x + b.x + c.x) / 3;
	m_Centroid.y = (a.y + b.y + c.y) / 3;
}
Triangle::Triangle(Edge & e, Vertice & a) {
	//criar um triangulo apartir de uma edge e um vertice, atualiza o vertice passado e cria 2 novos
	Vertice b = e.getV0();
	Vertice c = e.getV1();
	TriangleList stub;
	this->e0 = &e;
	this->e1 = new Edge(c,a);
	this->e2 = new Edge(a,b);
	setCircumCircle();
	m_Centroid.x = (a.x + b.x + c.x) / 3;
	m_Centroid.y = (a.y + b.y + c.y) / 3;
}
Triangle::~Triangle() {
	t_Mutex.unlock();
	t_Mutex.~mutex();
}
void Triangle::setCircumCircle() {
	try {
		float epsilon = 1.192092896e-07F;

		Vertice a = Edge::CommonVertice(*this->e2,*this->e0); //CA AB
		Vertice b = Edge::CommonVertice(*this->e0,*this->e1); //AB BC
		Vertice c = Edge::CommonVertice(*this->e1,*this->e2); //BC CA
		float x0 = a.x;
		float y0 = a.y;
		float x1 = b.x;
		float y1 = b.y;
		float x2 = c.x;
		float y2 = c.y;

		float dy10 = y1 - y0;
		float dy21 = y2 - y1;

		bool b21zero = dy21 > -epsilon && dy21 < epsilon;
		if (dy10 > -epsilon && dy10 < epsilon) {
			if (b21zero) { //todos 3 vertices na horizontal
				if(x1 > x0) {
					if (x2 > x1) x1 = x2;
				} else {
					if (x2 < x0) x0 = x2;
				}
				m_Center.x = (x0 + x1) * .5f;
				m_Center.y = (y0);
			} else { //vertices 0 e 1 estao na mesma linha horizontal
				float m1 = - (x2 - x1) / dy21;

				float mx1 = (x1 + x2) * .5f;
				float my1 = (y1 + y2) * .5f;

				m_Center.x = ((x0 + x1) * .5f);
				m_Center.y = (m1 * (m_Center.x - mx1) + my1);
			}
		} else if (b21zero) { // vertices 1 e 2 estao na mesma linha horizontal
			float m0 = - (x1 - x0) / dy10;

			float mx0 = (x0 + x1) * .5f;
			float my0 = (y0 + y1) * .5f;

			m_Center.x = ((x1 + x2) * .5f);
			m_Center.y = (m0 * (m_Center.x - mx0) + my0);
		} else { // vertices em linhas horizontais diferentes, caso mais "comum"
			float m0 = - (x1 - x0) / dy10;
			float m1 = - (x2 - x1) / dy21;

			float mx0 = (x0 + x1) * .5f;
			float my0 = (y0 + y1) * .5f;

			float mx1 = (x1 + x2) * .5f;
			float my1 = (y1 + y2) * .5f;

			m_Center.x = ((m0 * mx0 - m1 * mx1 + my1 - my0) / (m0 - m1));
			m_Center.y = (m0 * (m_Center.x - mx0) + my0);

		}
		float dx = x0 - m_Center.x;
		float dy = y0 - m_Center.y;

		m_R2 = dx * dx + dy * dy;	// the radius of the circumcircle, squared
		m_R = (float) sqrt(m_R2);	// the proper radius

		// Version 1.1: make m_R2 slightly higher to ensure that all edges
		// of co-circular vertices will be caught.
		// Note that this is a compromise. In fact, the algorithm isn't really
		// suited for very many co-circular vertices.
		m_R2 *= 1.000001f;

	} catch (int e) {
		std::cout << "Triangle not initialized" << std::endl;
	}
}
bool Triangle::CCEncompasses(const Vertice & v) const {
	float distX = v.x - m_Center.x;
	float distY = v.y - m_Center.y;
	float dist2 = distX*distX + distY*distY;		// squared
	return dist2 <= m_R2;							// compare with squared radius
}
bool Triangle::containsEdge(const Edge & e) const {
	try {
		if (e == getE0() || e == getE1() || e == getE2())
			return true;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return false;
}
bool Triangle::containsVertex(const Vertice & p) const {
	Vertice p1 = Edge::CommonVertice(*e0,*e1);
	Vertice p2 = Edge::CommonVertice(*e1,*e2);
	Vertice p3 = Edge::CommonVertice(*e2,*e0);
	float alpha = ((p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y)) /
	        ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
	float beta = ((p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y)) /
	       ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
	float gamma = 1.0f - alpha - beta;

	return (alpha > 0 && beta > 0 && gamma > 0);
}
bool Triangle::containsTriangle(const Triangle & t) const {
//	Vertice a = Edge::CommonVertice(*e0,*e1);
//	Vertice b = Edge::CommonVertice(*e1,*e2);
//	Vertice c = Edge::CommonVertice(*e2,*e0);

	return false;
}
Vertice Triangle::baryCoords(const Vertice & p) const {
	Vertice a = Edge::CommonVertice(*e0,*e1);
	Vertice b = Edge::CommonVertice(*e1,*e2);
	Vertice c = Edge::CommonVertice(*e2,*e0);
	float den = 1 / ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
	Vertice retorno;
	retorno.x = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) * den;
	retorno.y = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) * den;
	return retorno;
}
Vertice Triangle::getCentroid() const {
	return m_Centroid;
}
Triangle & Triangle::Triangulation(VerticeList & m_Vertices) {
	try {
		if (m_Vertices.size() < 3)
			throw 1;
		const Vertice vBegin = *m_Vertices.begin();
		float xMin = vBegin.x;
		float yMin = vBegin.y;
		float xMax = xMin;
		float yMax = yMin;

		for_each(m_Vertices.begin(), m_Vertices.end(), [&] (const Vertice & v ) {
			xMin = (v.x < xMin ? v.x : xMin);
			xMax = (v.x > xMax? v.x : xMax);
			yMin = (v.y < yMin ? v.y : yMin);
			yMax = (v.y > yMax? v.y : yMax);
		});
		float dx = xMax - xMin;
		float dy = yMax - yMin;

		float ddx = dx * 0.01f;
		float ddy = dy * 0.01f;

		xMin -= ddx;
		xMax += ddx;
		dx += 2 * ddx;

		yMin -= ddy;
		yMax += ddy;
		dy += 2 * ddy;

		const float sqrt3 = 1.732050808f;
		Vertice a(xMin - dy * sqrt3 / 3.0f, yMin);
		Vertice b(xMax + dy * sqrt3 / 3.0f, yMin);
		Vertice c((xMin + xMax) * 0.5f, yMax + dx * sqrt3 * 0.5f);

		return *(new Triangle(a,b,c));

	} catch (int e) {
		return *(new Triangle());
	}
}

Edge::Edge(Vertice _v0, Vertice _v1) {
	m_Vertices[0] = _v0;
	m_Vertices[1] = _v1;
	m_Triangles[0] = NULL;
	m_Triangles[1] = NULL;
}
void Edge::setAdjancency(Triangle & t) {
	try {
		if (m_Triangles[0] == NULL)
			m_Triangles[0] = &t;
		else
			m_Triangles[1] = &t;

	} catch (int e) {
	}
}
void Edge::setAdjancency(Triangle & t, int n) {
	try {
		if (n > 1)
			throw 1;
		m_Triangles[(n%2)] = &t;
	} catch (int e) {
		if (e == 1)
			std::cout << "Edge::setAdjancency : Out of boundries" << std::endl;
	}
}
void Edge::remAdjancency(Triangle & t) {
	try {
		for(int i = 0; i < 2; i++) {
			if (m_Triangles[i] == &t) {
				m_Triangles[i] = NULL;
			}
		}
	} catch (int e) {
		std::cout << "Edge::setAdjancency : Out of boundries" << std::endl;
	}
}
void Edge::remAdjancency(int n) {
	try {
		if (n > 1)
			throw 1;
		m_Triangles[(n%2)] = NULL;
	} catch (int e) {
		if (e == 1)
			std::cout << "Edge::setAdjancency : Out of boundries" << std::endl;
	}
}
Vertice Edge::CommonVertice(Edge & e1, Edge & e2) {
	Vertice a = e1.getV0();
	Vertice b = e1.getV1();
	Vertice c = e2.getV0();
	Vertice d = e2.getV1();

	if (a == c)
		return a;
	else if (a == d)
		return a;
	else if (b == c)
		return b;
	else if (b == d)
		return b;
	return * new Vertice();
}
bool Edge::hasCommomVertice(Edge & e) {
	Vertice a = getV0();
	Vertice b = getV1();
	Vertice c = e.getV0();
	Vertice d = e.getV1();

	if (a == c || a == d || b == c || b == d)
		return true;
	return false;
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
