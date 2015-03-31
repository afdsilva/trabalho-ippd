/*
 * DelaunayThread.cpp
 *
 *  Created on: 13/07/2014
 *      Author: andref
 */

#include "DelaunayThread.h"

void DelaunayThread::operator()(int nThread) {
		//std::cout << "Thread " << nThread << " iniciada " << std::endl;
		bool run = true;
		sf::Clock clock;
		while(run) {
			Vertice p;
			Triangle * t = NULL;
			Cavity * cavity = NULL;
			try {
				m_Mutex.lock();
				if (m_List.empty())
					throw 1; //termina execucao da thread
				p = m_List.front();
				m_List.pop_front();
				t = searchTriangle(p);
				m_Mutex.unlock();
				if (t == NULL)
					throw 2; // envia ponto de volta para fila, triangulo n encontrado
				cavity = new Cavity(*t,p);
				if (cavity->expand()) {
					if (!cavity->retriangulate())
						throw 3; // envia o ponto de volta para fila, cavidade com problema
					m_Mesh.updateGraph(*cavity,m_Mutex);
					m_Mutex.lock();
					m_Mesh.updateList();
					m_Mutex.unlock();
				} else {
					throw 4; //cavidade em uso
				}
				cavity->unlockEdges();
				cavity->unlockTriangles();
			} catch (int e) {
				if (e == 1) {
					run = false;
				} else {
					if (e == 2) {
						std::cout << "Triangulo não encontrado" << std::endl;
					} else {
						if (e == 3) {
							std::cout << "Cavidade com problema" << std::endl;
							if (p.cowjuggling)
								run = false;
							p.cowjuggling = true;
						}
						if (e == 4) {
//							std::cout << "Cavidade em uso" << std::endl;
						}
						if (!(cavity == NULL)) {
//							std::cout << "Destravando cavidade" << std::endl;
							cavity->unlockEdges();
							cavity->unlockTriangles();
						}
					}
//					std::cout << "Enviando Ponto de volta para fila" << std::endl;
					m_Mutex.lock();
					m_List.push_back(p);
				}
				m_Mutex.unlock();
			}
			m_Mutex.lock();
			m_ElapsedTime = clock.getElapsedTime();
			m_Mutex.unlock();
		}
		clock.restart();
		//std::cout << "Thread " << nThread << " finalizada" << std::endl;

	}
Triangle * DelaunayThread::searchTriangle(const Vertice & v) {
	try {
		//procura o vertice na lista, apenas um triangulo pode conter o ponto, entao
		//trava o triangulo caso o ponto encontre, caso o triangulo jah esteja com lock
		//retorna NULL e procura por outra cavidade
//		std::cout << v << std::endl;
		TriangleList m_Triangles;
		if (!v.cowjuggling) { //acontece que as vezes nao consegue encontrar o objeto que "presta" e fica num loop infinito, esse drible da vaca deve solucionar esse problema
			m_Triangles = m_Quadtree.GetObjectsAt(v.x,v.y);

			std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) {
	//			std::cout << "Encontrado containsVertex: " << std::endl << *t << std::endl;
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
	//			std::cout << "Encontrado CCEncompasses: " << std::endl << *t << std::endl;
				if (t && t->CCEncompasses(v)) {
					throw t;
					if (t->r_Mutex.try_lock())
						throw t;
					else
						throw 2;
				}
			});
		}
		//se falha em encontrar da maneira rapida um triangulo q sirva, faz da maneira lenta :v
		m_Triangles = m_Quadtree.GetObjects();
		std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) {
			if (t && t->containsVertex(v)) {
				throw t;
				if (t->r_Mutex.try_lock())
					throw t;
				else
					throw 3;
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
					throw 4;
			}
		});
	} catch (Triangle * r) {
		return r;
	} catch (int e) {
			std::cout << "Triangulo jah possui lock " << e << std::endl;
	}
	return NULL;
}
Graph & DelaunayThread::ExecuteThread(VerticeList & vertices, int nthreads, int depth) {
	Graph * m_Mesh = NULL;
	try {
		std::cout << "Iniciando Algorito Threads C++11" << std::endl;
		std::list<Vertice> m_VerticeList = std::list<Vertice>(vertices.begin(),vertices.end());
		Triangle * descritor = &Triangle::Triangulation(vertices);

		Quadtree * quadTree = new Quadtree(0.0f,0.0f,1024.0f,768.0f,0,depth);
		quadTree->AddObject(descritor);
		m_Mesh = new Graph(*descritor, *quadTree);
		std::vector<std::thread> threads;
		sf::Clock clock;
		clock.restart();
		std::mutex mutex;
		sf::Time totalElapsed;
		DelaunayThread dt(m_VerticeList,*quadTree,*m_Mesh,mutex,totalElapsed);
		for (int i = 0; i < nthreads; i++) {
			threads.push_back(std::thread(dt,i));
		}
		for(auto & thread : threads)
			thread.join();
		cout << "Tempo do Algoritmo " << totalElapsed.asSeconds() << " Entrada: " << vertices.size() << " Threads: " << nthreads << std::endl;

	} catch (int e) {

	}

	return *m_Mesh;
}


bool Cavity::expand() {
	m_TrianglesCavity->clear();
	m_EdgesList->clear();
	m_EdgesLock->clear();
	return expand(m_Triangle);
}
bool Cavity::expand(Triangle & t) {
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
bool Cavity::retriangulate() {
	bool retorno = true;
	try {
		m_TriangleSet->clear();
		EdgeList nEdges;

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
		if (m_TriangleSet->size() != m_TrianglesCavity->size()+2) {
			throw 1;

		}
		retorno = true;
	} catch (std::exception & e) {
		retorno = false;

	} catch (int e) {
//		int setsize = m_TriangleSet->size();
//		int cavitysize = m_TrianglesCavity->size();
//		int edgessize = m_EdgesList->size();
//		std::cout << "Problema na retriangulacao set = " << setsize << " cavity = " << cavitysize << " edge = " << edgessize << std::endl;
		retorno = false;
	}
	return retorno;
}
TriangleList & Cavity::getNewTriangles() const {
	//retorna as edges, usado no updatemesh, para corrigir os triangulos modificados
	return *m_TriangleSet;
}
TriangleList & Cavity::getModifiedTriangles() const {
	//retorna as edges, usado no updatemesh, para corrigir os triangulos modificados
	return *m_TrianglesCavity;
}
bool Cavity::lockEdges(std::mutex & mutex) {
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
bool Cavity::lockTriangles(std::mutex & mutex) {
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
void Cavity::unlockEdges() {
	std::for_each(m_EdgesLock->begin(), m_EdgesLock->end(), [&] (Edge * e){
		e->e_Mutex.unlock();
	});
}
void Cavity::unlockTriangles() {
	std::for_each(m_TrianglesCavity->begin(), m_TrianglesCavity->end(), [&] (Triangle * t){
		t->t_Mutex.unlock();
	});
	std::for_each(m_TriangleSet->begin(), m_TriangleSet->end(), [&] (Triangle * t){
		t->t_Mutex.unlock();
	});
}


void Graph::updateGraph(Cavity & c, std::mutex & mutex) {
	//atualizar o grafo requer o seguintes passos:
	//atualizar o descritor
	//remover a adjacencia das edges dos triangulos modificados (remove a referencia para eles)
	bool modifica = false;
	mutex.lock();
//	if (c.getModifiedTriangles().size() + 2 != c.getNewTriangles().size())
//		std::cout << "Problema" << std::endl;

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
	TriangleList newTris = c.getNewTriangles();
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
	mutex.unlock();

}
void Graph::updateList() {
	m_Quadtree.Clear();
	m_TrianglesList->clear();
	updateList(*m_Descritor);
	std::for_each(m_TrianglesList->begin(), m_TrianglesList->end(),[&] (Triangle * t) {
		t->n_Mutex.unlock();
	});
}
void Graph::updateList(Triangle & t) {
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
Triangle & Graph::getDescritor() const {
	return * m_Descritor;
}
TriangleList & Graph::getTriangleList() const {
	return * m_TrianglesList;
}
Triangle * Graph::searchTriangle(const Vertice & v) const {
	TriangleList m_Triangles = m_Quadtree.GetObjectsAt(v.x, v.y);
	try {
		std::for_each(m_Triangles.begin(), m_Triangles.end(), [&] (Triangle * t) {
			if (t && t->r_Mutex.try_lock()) {
				if (t->containsVertex(v))
					throw t;
				}
				t->r_Mutex.unlock();
		});
	} catch (Triangle * t) {
		return t;
	}
	return NULL;
}
