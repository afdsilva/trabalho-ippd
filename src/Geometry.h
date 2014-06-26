/*
 * Geometry.h
 *
 *  Created on: 25/06/2014
 *      Author: andref
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <set>

using namespace std;
class Geometry {
};

class Vertex;
class Edge;
class Triangle;

class Vertex {
public:
	Vertex()								: m_Pnt(0.0f, 0.0f), id(0)				{}
	Vertex(const Vertex& v)					: m_Pnt(v.m_Pnt), id(v.id)				{}
	Vertex(const sf::Vector2f& pnt, int i)	: m_Pnt(pnt), id(i)						{}
	Vertex(float x, float y, int i)			: m_Pnt(x, y), id(i)					{}
	Vertex(int x, int y, int i)				: m_Pnt((float) x, (float) y), id(i)	{}

	bool operator<(const Vertex& v) const {
		if (m_Pnt.x == v.m_Pnt.x) return m_Pnt.y < v.m_Pnt.y;
		return m_Pnt.x < v.m_Pnt.x;
	}

	bool operator==(const Vertex& v) const {
		return m_Pnt.x == v.m_Pnt.x && m_Pnt.y == v.m_Pnt.y;
	}

	float GetX()	const	{ return m_Pnt.x; }
	float GetY() 	const	{ return m_Pnt.y; }

	void SetX(float x)		{ m_Pnt.x = x; }
	void SetY(float y)		{ m_Pnt.y = y; }

	int GetId() 	const 	{ return id;}

	const sf::Vector2f& GetPoint() const		{ return m_Pnt; }
	void show(string msg) const { cout << msg << " " << id << " x: " << m_Pnt.x <<  " y: " << m_Pnt.y << endl; }


protected:

	sf::Vector2f m_Pnt;
	int id;
};

class Triangle : public sf::Drawable, public sf::Transformable  {
public:
	//Triangle(const Triangle & t) : vertices(t.vertices), m_Center(t.m_Center), m_R(t.m_R), m_R2(t.m_R2), shape(t.shape) {}
	Triangle(Vertex * p0, Vertex * p1, Vertex * p2) {
		vertices[0] = p0;
		vertices[1] = p1;
		vertices[2] = p2;

		shape = sf::VertexArray(sf::Triangles,3);
		shape[0] = sf::Vertex(sf::Vector2f(p0->GetX(),p0->GetY()));
		shape[1] = sf::Vertex(sf::Vector2f(p1->GetX(),p1->GetY()));
		shape[2] = sf::Vertex(sf::Vector2f(p2->GetX(),p2->GetY()));
		setCircumCircle();
	}

	Vertex * getVertex(int i) const {
		try {
			if (i < 0 || i >= 3)
				throw;
			Vertex * v = vertices[i];
			return v;
		} catch (int e) {
			return NULL;
		}
	}

	sf::Vertex * getShape(int i) {
		try {
			if (i < 0 || i >= 3)
				throw;
			return &shape[i];
		} catch (int e) {
			return NULL;
		}

	}
	bool operator<(const Triangle& tri) const {
		if (m_Center.GetX() == tri.m_Center.GetX()) return m_Center.GetY() < tri.m_Center.GetY();
		return m_Center.GetX() < tri.m_Center.GetX();
	}
	bool operator==(const Triangle& tri) const {
		return (m_Center.GetX() == tri.m_Center.GetX()) && (m_Center.GetY() == tri.m_Center.GetY());
	}
	bool isLeftOf(const Vertex & v) const {
		// returns true if is to the right of the triangle's circumcircle
		return v.GetX() > (m_Center.GetX() + m_R);
	}
	//Verifica se o vertice esta dentro do circumcircle do triangulo
	bool CCEncompasses(const Vertex * v) const {
		float distX = v->GetX() - m_Center.GetX();
		float distY = v->GetY() - m_Center.GetY();
		float dist2 = distX*distX + distY*distY;		// squared
		return dist2 <= m_R2;							// compare with squared radius
	}

	float pointInTriangle(const Vertex * v) const {

		float x = v->GetX();
		float y = v->GetY();

		Vertex * v0 = vertices[0];
		Vertex * v1 = vertices[1];
		Vertex * v2 = vertices[2];

		float x1 = v0->GetX();
		float y1 = v0->GetY();
		float x2 = v1->GetX();
		float y2 = v1->GetY();
		float x3 = v2->GetX();
		float y3 = v2->GetY();

		float  denominator = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
		float a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / denominator;
		float b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / denominator;
		float c = 1 - a - b;

	return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1;
	}
	Vertex getCenter() {
		return m_Center;
	}
	void show() const {
		for(int i = 0; i < 3; i++)
			vertices[i]->show("Triangulo ");
	}
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

//			sf::CircleShape m_circle;
//			m_circle.setPosition(sf::Vector2f(m_Center.GetX(),m_Center.GetY()));
//			m_circle.setOutlineColor(sf::Color::Red);
//			m_circle.setOutlineThickness(1);
//			//m_circle.setFillColor(sf::Color::Blue);
//			m_circle.setRadius(m_R);
//			m_circle.setOrigin(sf::Vector2f(m_R, m_R));
//			target.draw(m_circle, states);

			target.draw(shape, states);
	}

	Triangle * neighbors[3];
	Vertex * vertices[3];

	sf::VertexArray shape;
	Vertex m_Center;
	float m_R;
	float m_R2;

	void setCircumCircle() {
		float epsilon = 1.192092896e-07F;

		float x0 = vertices[0]->GetX();
		float y0 = vertices[0]->GetY();
		float x1 = vertices[1]->GetX();
		float y1 = vertices[1]->GetY();
		float x2 = vertices[2]->GetX();
		float y2 = vertices[2]->GetY();

//		float ax = vertices[0]->GetX();
//		float ay = vertices[0]->GetY();
//		float bx = vertices[1]->GetX();
//		float by = vertices[1]->GetY();
//		float cx = vertices[2]->GetX();
//		float cy = vertices[2]->GetY();
//
//		float d = 2.0f * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
//		float ux = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by + by) * (cy - ay) + ( cx * cx + cy * cy) * (ay - by)) / d;
//		float yx = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by + by) * (ax - cx) + ( cx * cx + cy * cy) * (bx - ax)) / d;
//
//		m_Center.SetX(ux);
//		m_Center.SetY(yx);
//
//		m_R2 = d;	// diametro
//		m_R = (float) sqrt(d);	// the proper radius
//
//		return;

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
				m_Center.SetX((x0 + x1) * .5f);
				m_Center.SetY(y0);
			} else { //vertices 0 e 1 estao na mesma linha horizontal
				float m1 = - (x2 - x1) / dy21;

				float mx1 = (x1 + x2) * .5f;
				float my1 = (y1 + y2) * .5f;

				m_Center.SetX((x0 + x1) * .5f);
				m_Center.SetY(m1 * (m_Center.GetX() - mx1) + my1);
			}
		} else if (b21zero) { // vertices 1 e 2 estao na mesma linha horizontal
			float m0 = - (x1 - x0) / dy10;

			float mx0 = (x0 + x1) * .5f;
			float my0 = (y0 + y1) * .5f;

			m_Center.SetX((x1 + x2) * .5f);
			m_Center.SetY(m0 * (m_Center.GetX() - mx0) + my0);
		} else { // vertices em linhas horizontais diferentes, caso mais "comum"
			float m0 = - (x1 - x0) / dy10;
			float m1 = - (x2 - x1) / dy21;

			float mx0 = (x0 + x1) * .5f;
			float my0 = (y0 + y1) * .5f;

			float mx1 = (x1 + x2) * .5f;
			float my1 = (y1 + y2) * .5f;

			m_Center.SetX((m0 * mx0 - m1 * mx1 + my1 - my0) / (m0 - m1));
			m_Center.SetY(m0 * (m_Center.GetX() - mx0) + my0);

		}
		float dx = x0 - m_Center.GetX();
		float dy = y0 - m_Center.GetY();

		m_R2 = dx * dx + dy * dy;	// the radius of the circumcircle, squared
		m_R = (float) sqrt(m_R2);	// the proper radius

		// Version 1.1: make m_R2 slightly higher to ensure that all edges
		// of co-circular vertices will be caught.
		// Note that this is a compromise. In fact, the algorithm isn't really
		// suited for very many co-circular vertices.
		m_R2 *= 1.000001f;
	}

};

class Edge : public sf::Drawable, public sf::Transformable {
public:
	Edge(const Edge & e) : m_pV0(e.m_pV0), m_pV1(e.m_pV1), neighbor(e.neighbor) {

		shape = sf::VertexArray(sf::Lines,2);
		shape[0] = sf::Vertex(sf::Vector2f(m_pV0->GetX(),m_pV0->GetY()));
		shape[1] = sf::Vertex(sf::Vector2f(m_pV1->GetX(),m_pV1->GetY()));
	}
	Edge(const Vertex * v0, const Vertex * v1) : m_pV0(v0), m_pV1(v1), neighbor(NULL) {
		shape = sf::VertexArray(sf::Lines,2);
		shape[0] = sf::Vertex(sf::Vector2f(m_pV0->GetX(),m_pV0->GetY()));
		shape[1] = sf::Vertex(sf::Vector2f(m_pV1->GetX(),m_pV1->GetY()));
	}
	Edge(const Vertex * v0, const Vertex * v1, const Triangle &n) : m_pV0(v0), m_pV1(v1), neighbor(&n) {
		shape = sf::VertexArray(sf::Lines,2);
		shape[0] = sf::Vertex(sf::Vector2f(m_pV0->GetX(),m_pV0->GetY()));
		shape[1] = sf::Vertex(sf::Vector2f(m_pV1->GetX(),m_pV1->GetY()));
	}
	bool operator<(const Edge& e) const {
		if (m_pV0 == e.m_pV0) {
			return * m_pV1 < * e.m_pV1;
		}
		return * m_pV0 < * e.m_pV0;
	}
	bool operator==(const Edge& e) const {
		//AB == BA
		if (m_pV0 == m_pV1) return true;
		return m_pV1 == m_pV0;
	}
	void show() const {
		m_pV0->show("Edge 0: ");
		m_pV1->show("Edge 1: ");
	}

	const Vertex * m_pV0;
	const Vertex * m_pV1;
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
		target.draw(shape, states);
	}
	const Triangle * neighbor;
	sf::VertexArray shape;
};

class drawTriangle {
public:
	drawTriangle(sf::RenderWindow & w) : window(w) {}
	void operator()(const Triangle &tri) const {
		Triangle * t = (Triangle *)&tri;
		sf::Vertex * v0 = t->getShape(0);
		sf::Vertex * v1 = t->getShape(1);
		sf::Vertex * v2 = t->getShape(2);
		v0->color = sf::Color::Red;
		v1->color = sf::Color::Blue;
		v2->color = sf::Color::Green;

		//t.getShape(0).
		window.draw(tri);
	}
protected:

	sf::RenderWindow & window;
};

class drawVertex {
public:
	drawVertex(sf::RenderWindow & w) : window(w) {}
	void operator()(const Vertex *v) const {
		sf::CircleShape m_circle;
		m_circle.setPosition(sf::Vector2f(v->GetX(),v->GetY()));
		m_circle.setRadius(2);
		window.draw(m_circle);
	}
protected:

	sf::RenderWindow & window;
};

class drawEdge {
public:
	drawEdge(sf::RenderWindow & w) : window(w) {}
	void operator()(const Edge &e) const {
		window.draw(e);
	}
protected:

	sf::RenderWindow & window;
};

#endif /* GEOMETRY_H_ */
