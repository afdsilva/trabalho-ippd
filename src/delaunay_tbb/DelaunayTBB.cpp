/*
 * DelaunayTBB.cpp
 *
 *  Created on: 15/07/2014
 *      Author: andref
 */


#include <iostream>

#include "DelaunayTBB.h"

void DelaunayTBB::operator()( const tbb::blocked_range<size_t>& r ) const {
	try {
		//std::cout << "Task Iniciada" << std::endl;
		for(unsigned int i = r.begin(); i != r.end(); ++i) {
//			std::cout << m_Vertices[i] << std::endl;
			bool run = true;
			while(run) {
				m_Mutex.lock();
				Triangle * t = m_Mesh.searchTriangle(m_Vertices[i]);
				m_Mutex.unlock();
				if (t == NULL)
					throw 1;
				CavityTBB * cavity = new CavityTBB(*t,m_Vertices[i]);
				if (cavity->expand()) {
					cavity->retriangulate();
					m_Mesh.updateGraph(*cavity);
					m_Mutex.lock();
					m_Mesh.updateList();
					m_Mutex.unlock();
					run = false;
				}
				cavity->unlockEdges();
				cavity->unlockTriangles();
			}
		}
	} catch (std::exception & e) {
	} catch (int e) {
//		std::cout << "Triangulo nao encontrado" << std::endl;
	}
}
TriangleList & DelaunayTBB::ExecuteTBB(VerticeList & vertices, int threads) {
	std::cout << "Iniciando Algoritmo TBB" << std::endl;
//	GenerateVertices ver(n_Entries,Vertice::listVertices);
	tbb::task_scheduler_init init(threads);
	sf::Clock clock;
	Quadtree * quadtree = new Quadtree(0,0,1024,768,0,3);
	GraphTBB * m_Mesh = new GraphTBB(Triangle::Triangulation(vertices),*quadtree);
	tbb::mutex mutex;
	tbb::parallel_for(tbb::blocked_range<size_t>(0, vertices.size()),DelaunayTBB(vertices,*m_Mesh,mutex));
	sf::Time elapsed = clock.getElapsedTime();
	clock.restart();

	std::cout << "Tempo do Algoritmo " << elapsed.asSeconds() << " Entrada: " << vertices.size() << " Threads " << threads << std::endl;
	return m_Mesh->getTriangleList();

}

bool CavityTBB::expand() {
	m_TrianglesCavity->clear();
	m_EdgesList->clear();
	m_EdgesLock->clear();
	return expand(m_Triangle);
}
bool CavityTBB::expand(Triangle & t) {
	bool retorno = true;

	try {
		std::list<Triangle *> filaTriangulos;

		filaTriangulos.push_back(&t);
		while(filaTriangulos.size() > 0) {
			Triangle * nt = filaTriangulos.front();
			filaTriangulos.pop_front();
			itTriangles found = std::find(m_TrianglesCavity->begin(), m_TrianglesCavity->end(), nt);
			if (found == m_TrianglesCavity->end()) {
				if (nt->CCEncompasses(m_Vertice)) {
					if (!nt->t_Mutex.try_lock())
						throw 1; //nao conseguiu pegar o lock do triangulo, cavidade em uso
					m_TrianglesCavity->push_back(nt);
					Edge * e[3];
					e[0] = &nt->getE0();
					e[1] = &nt->getE1();
					e[2] = &nt->getE2();

					for (int i = 0; i < 3; i++) {
						itEdges found = std::find(m_EdgesList->begin(), m_EdgesList->end(), e[i]);
						if (found == m_EdgesList->end()) {
							m_EdgesList->push_back(e[i]);
							if (!e[i]->e_Mutex.try_lock())
									throw 2; //nao conseguiu pegar lock da edge, cavidade em uso
							m_EdgesLock->push_back(e[i]);
						} else {
							m_EdgesList->erase(found);
						}
						if (e[i]->getT1()) {
							filaTriangulos.push_back(e[i]->getT1());
						}
						if (e[i]->getT2()) {
							filaTriangulos.push_back(e[i]->getT2());
						}
					}
				}
			}
		}
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
		retorno = false;
	} catch (int e) {
		retorno = false;
//		std::cout << "Cavity::expand EXCECAO: " << e << std::endl;
	}
	return retorno;
}
void CavityTBB::retriangulate() {
	m_TriangleSet->clear();
	EdgeList nEdges;
//	std::cout << "m_EdgesList: " << m_EdgesList->size() << " Cavity Size: " << m_TrianglesCavity->size() << std::endl;

	std::for_each(m_EdgesList->begin(),m_EdgesList->end(), ([&] (Edge * c_Edge) {

		//cria novas edges
		//AB -> P = AP, BP => ABP
		//mas AP e BP vao ser usados nas outras edges tambem

		Vertice a = c_Edge->getV0();
		Vertice b = c_Edge->getV1();
		nEdges.push_back(c_Edge);
		Edge * ap = new Edge(a,m_Vertice);
		Edge * bp = new Edge(b,m_Vertice);
		//verifica se a edge jah n esta na lista
		bool apv = false, bpv = false;
		std::for_each(nEdges.begin(), nEdges.end(),[&](Edge * e) {
			//parece esquisito mas eh pra colocar o ap e o bp como referencia caso jah esteja na lista
			if (*ap == *e) {
				apv = true;
				ap = e;
			}
			if (*bp == *e) {
				bpv = true;
				bp = e;
			}

		});
		//caso n tenha encontrado alguma das edges na lista, coloca
		ap->e_Mutex.try_lock();
		m_EdgesLock->push_back(ap);
		bp->e_Mutex.try_lock();
		m_EdgesLock->push_back(bp);
		if (!apv)
			nEdges.push_back(ap);
		if (!bpv)
			nEdges.push_back(bp);
		//cria o novo triangulo com as edges
		Triangle * nTri = new Triangle(*c_Edge,*ap,*bp);
		nTri->t_Mutex.lock();

		//insere na lista
		m_TriangleSet->push_back(nTri);
	}));
//	if (m_TriangleSet->size() != m_TrianglesCavity->size()+2) {
//		int setsize = m_TriangleSet->size();
//		int cavitysize = m_TrianglesCavity->size();
//		int edgessize = m_EdgesList->size();
//		std::cout << "Problema na retriangulacao set = " << setsize << " cavity = " << cavitysize << " edge = " << edgessize << std::endl;
//
//	}

}
TriangleList & CavityTBB::getNewTriangles() const {
	//retorna as edges, usado no updatemesh, para corrigir os triangulos modificados
	return *m_TriangleSet;
}
TriangleList & CavityTBB::getModifiedTriangles() const {
	//retorna as edges, usado no updatemesh, para corrigir os triangulos modificados
	return *m_TrianglesCavity;
}
bool CavityTBB::lockEdges(tbb::mutex & mutex) {
	bool retorno = true;
	EdgeList tmp;
	mutex.lock();
	try {
		std::for_each(m_EdgesLock->begin(), m_EdgesLock->end(), [&] (Edge * e){
			//caso nao consiga adquirir algum dos locks, vai retornar falso
			if (e->e_Mutex.try_lock()) {
				tmp.push_back(e);
			} else {
				throw 1;
			}
		});
	} catch (int e) {
//		std::cout << "Cancelando transacao" << std::endl;
		std::for_each(tmp.begin(), tmp.end(), [&] (Edge * e){
			e->e_Mutex.unlock();
		});
		retorno = false;
	}
	mutex.unlock();
	return retorno;
}
bool CavityTBB::lockTriangles(tbb::mutex & mutex) {
	bool retorno = true;
	TriangleList tmp;
	mutex.lock();
	try {
		std::for_each(m_TrianglesCavity->begin(),m_TrianglesCavity->end(), [&] (Triangle * t) {
			if (t->t_Mutex.try_lock())
				tmp.push_back(t);
			else
				throw 1;
		});
	} catch (int e) {
		std::for_each(tmp.begin(), tmp.end(), [&] (Triangle * t){
			t->t_Mutex.unlock();
		});
		retorno = false;
	}
	mutex.unlock();
	return retorno;
}
void CavityTBB::unlockEdges() {
	std::for_each(m_EdgesLock->begin(), m_EdgesLock->end(), [&] (Edge * e){
		e->e_Mutex.unlock();
	});
}
void CavityTBB::unlockTriangles() {
	std::for_each(m_TrianglesCavity->begin(), m_TrianglesCavity->end(), [&] (Triangle * t){
		t->t_Mutex.unlock();
	});
	std::for_each(m_TriangleSet->begin(), m_TriangleSet->end(), [&] (Triangle * t){
		t->t_Mutex.unlock();
	});
}


void GraphTBB::updateGraph(CavityTBB & c) {
	bool modifica = false;
	std::for_each(c.getModifiedTriangles().begin(), c.getModifiedTriangles().end(), [&] (Triangle * t) {
		//pra cada triangulo, procura a referencia dele proprio nas edges, se o triangulo entrou na cavidade quer dizer que ele nao sera mais usado
		t->getE0().remAdjancency(*t);
		t->getE1().remAdjancency(*t);
		t->getE2().remAdjancency(*t);
		if (t == m_Descritor) {
			modifica = true;
		}
	});
	//modificar a adjacencia das edges dos novos triangulos (adicionar a referencia para eles)
	std::for_each(c.getNewTriangles().begin(), c.getNewTriangles().end(), [&] (Triangle * t) {
		//como as referencias jah sao removidas a partir dos triangulos, nao precisa mais trocar a referencia, pode ser colocado em qualquer posicao vazia
		//no caso testa a 1a, se ela for vazia coloca nela, do contrario, por eliminacao coloca na 2a posicao
		//se por algum motivo esse teste falha eh pq a logica ta errada e tem q modificar em algum outro lugar (to soh ponderando comigo mesmo)

		t->getE0().setAdjancency(*t);
		t->getE1().setAdjancency(*t);
		t->getE2().setAdjancency(*t);
	});
	if (modifica) {
		Triangle * t = c.getNewTriangles().at(0);
		m_Descritor = t;
	}

}
void GraphTBB::updateList() {
	m_Quadtree.Clear();
	m_TrianglesList->clear();
	updateList(*m_Descritor);
	std::for_each(m_TrianglesList->begin(), m_TrianglesList->end(),[&] (Triangle * t) {
		t->n_Mutex.unlock();
	});
}
void GraphTBB::updateList(Triangle & t) {
	try {
		if (!t.n_Mutex.try_lock())
			throw 1;
		m_Quadtree.AddObject(&t);
		m_TrianglesList->push_back(&t);

		Edge * e[3];
		e[0] = &t.getE0();
		e[1] = &t.getE1();
		e[2] = &t.getE2();
		for (int i = 0; i < 3; i++){
			if (e[i]->getT1() && e[i]->getT1() != &t) {
				updateList(*e[i]->getT1());
			}else if (e[i]->getT2() && e[i]->getT2() != &t) {
				updateList(*e[i]->getT2());
			}

		}
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;

	} catch (int e) {
//		std::cout << "Triangulo ja Inserido" << std::endl;
	}
}
Triangle & GraphTBB::getDescritor() const {
	return * m_Descritor;
}
TriangleList & GraphTBB::getTriangleList() const {
	return * m_TrianglesList;
}
Triangle * GraphTBB::searchTriangle(const Vertice & v) {
	try {
		//procura o vertice na lista, apenas um triangulo pode conter o ponto, entao
		//trava o triangulo caso o ponto encontre, caso o triangulo jah esteja com lock
		//retorna NULL e procura por outra cavidade
//		std::cout << v << std::endl;
		TriangleList m_Triangles = m_Quadtree.GetObjectsAt(v.x,v.y);

		std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) { if (t && t->containsVertex(v)) throw t; });
		//caso nao encontre nenhum triangulo que contenha o ponto (calculo do baricentro n funciona)
		//aih procura qualquer triangulo que tenha o ponto circunscrito
		std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) { if (t && t->CCEncompasses(v)) throw t; });

		//se falha em encontrar pela quadtree, procura em todos triangulos (possivelmente dah pra otimizar a quadtree, soh nao vou ter tempo pra isso
		m_Triangles = m_Quadtree.GetObjects();
		std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) { if (t && t->CCEncompasses(v)) throw t; });
	} catch (Triangle * r) {
		return r;
	} catch (int e) {
	}
	return NULL;
}
