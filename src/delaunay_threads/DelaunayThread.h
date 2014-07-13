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

#include "../Graph.h"

class Delaunay {
public:
	Delaunay(TriangleList & triangles, Graph & mesh, std::mutex & mutex) : m_Triangles(triangles), m_Mesh(mesh), m_Mutex(mutex) {}
	void operator()(itVertices begin, itVertices end) {
		std::for_each(begin,end,[&](const Vertice & v){
			bool run = true;
			while(run) {
//				std::cout << "Pegando cavidade" << std::endl;
				m_Mutex.lock(); //operacao atomica
				Triangle * search = searchTriangle(v);
				m_Mutex.unlock();
				Cavity * cavity = NULL;
				if (search) {
					cavity = new Cavity(*search,v);
					cavity->expand();
					cavity->retriangulate();

					if (cavity && cavity->lockEdges(m_Mutex)) {
						m_Mutex.lock();
						m_Mesh.updateGraph(*cavity, m_Mutex);
						m_Triangles = m_Mesh.updateList();
						run = false;
						cavity->unlockEdges();
						m_Mutex.unlock();
					} else {
						search->r_Mutex.unlock();
					}
				}


//				m_Mutex.lock();
//				m_Triangles = m_Mesh.updateList();
//				m_Mutex.unlock();
			}
		});

	}
protected:
	Triangle * searchTriangle(const Vertice & v) {
		try {
			//procura o vertice na lista, apenas um triangulo pode conter o ponto, entao
			//trava o triangulo caso o ponto encontre, caso o triangulo jah esteja com lock
			//retorna NULL e procura por outra cavidade
			std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) {
				if (t && t->containsVertex(v)) {
					if (t->r_Mutex.try_lock())
						throw t;
					else
						throw 1;
				}
			});
			//caso nao encontre nenhum triangulo que contenha o ponto (calculo do baricentro n funciona)
			//aih procura qualquer triangulo que tenha o ponto circunscrito
			std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) {
				if (t && t->CCEncompasses(v)) {
					if (t->r_Mutex.try_lock())
						throw t;
					else
						throw 2;
				}
			});
		} catch (Triangle * r) {
			return r;
		} catch (int e) {
//			std::cout << "Triangulo jah possui lock " << e << std::endl;
		}
		return NULL;
	}
	TriangleList & m_Triangles;
	Graph & m_Mesh;
	std::mutex & m_Mutex;
};

class DelaunayConsumer {
public:
	DelaunayConsumer(std::list<Vertice> & list, TriangleList & triangles, Graph & mesh, std::mutex & mutex) : m_List(list), m_Triangles(triangles), m_Mesh(mesh), m_Mutex(mutex) {}
	void operator()(int nThread) {
//		std::cout << "Thread " << nThread << " iniciada " << std::endl;
		while(!m_List.empty()) {
//			std::cout << "lista: " << m_List.size() << std::endl;
			Vertice p;
			Triangle * t = new Triangle();
			try {
				m_Mutex.lock();
				if (m_List.empty())
					throw 3;
				p = m_List.front();
//				std::cout << "Thread " << nThread << " processando: " << p << std::endl;
				Triangle * t = searchTriangle(p);
				m_List.pop_front();
				m_Mutex.unlock();
				if (t == NULL) {
					throw 1;
				}
				Cavity * cavity = new Cavity(*t,p);
				cavity->expand();
				if (cavity->lockEdges(m_Mutex)) {
					cavity->retriangulate();
					m_Mesh.updateGraph(*cavity,m_Mutex);
					m_Mutex.lock();
					m_Triangles = m_Mesh.updateList();
					m_Mutex.unlock();
					cavity->unlockEdges();
				} else
					throw 2;
			} catch (int e) {
//				std::cout << "Enviando Ponto de volta para fila" << std::endl;
				if (e == 3) {
					std::cout << "Lista Vazia" << std::endl;
				} else {
					m_Mutex.lock();
					m_List.push_back(p);
					m_Triangles = m_Mesh.updateList();
					m_Mutex.unlock();
				}
				if (e == 1) {
//					std::cout << "Triangulo invalido" << std::endl;
				}
				if (e == 2) {
					t->r_Mutex.unlock();
//					std::cout << "Cavidade em uso" << std::endl;
				}
			}
		}


	}
protected:
	Triangle * searchTriangle(const Vertice & v) {
		try {
			//procura o vertice na lista, apenas um triangulo pode conter o ponto, entao
			//trava o triangulo caso o ponto encontre, caso o triangulo jah esteja com lock
			//retorna NULL e procura por outra cavidade
			std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) {
				if (t && t->containsVertex(v)) {
					throw t;
					if (t->r_Mutex.try_lock())
						throw t;
					else
						throw 1;
				}
			});
			//caso nao encontre nenhum triangulo que contenha o ponto (calculo do baricentro n funciona)
			//aih procura qualquer triangulo que tenha o ponto circunscrito
			std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) {
				if (t && t->CCEncompasses(v)) {
					throw t;
					if (t->r_Mutex.try_lock())
						throw t;
					else
						throw 2;
				}
			});
		} catch (Triangle * r) {
			return r;
		} catch (int e) {
//			std::cout << "Triangulo jah possui lock " << e << std::endl;
		}
		return NULL;
	}

	std::list<Vertice> & m_List;
	TriangleList & m_Triangles;
	Graph & m_Mesh;
	std::mutex & m_Mutex;
};

#endif /* DELAUNAYTHREAD_H_ */
