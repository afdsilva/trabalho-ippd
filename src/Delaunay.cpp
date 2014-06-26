#include "Delaunay.h"

const float sqrt3 = 1.732050808f;

class vertexIsInCC {
public:
	vertexIsInCC(const Vertex * v, edgeSet & output) : vertex(v), m_Edges(output) {}
	bool operator()(const Triangle& tri) {
		bool retorno = tri.CCEncompasses(vertex);
		if (retorno) {
			handleEdge(tri.getVertex(0),tri.getVertex(1));
			handleEdge(tri.getVertex(1),tri.getVertex(2));
			handleEdge(tri.getVertex(2),tri.getVertex(0));
		}
		return retorno;
	}
protected:
	void handleEdge(const Vertex * p0, const Vertex * p1) const {
		const Vertex * pVertex0(NULL);
		const Vertex * pVertex1(NULL);
		if (*p0 < *p1) {
			pVertex0 = p0;
			pVertex1 = p1;
		} else {
			pVertex0 = p1;
			pVertex1 = p0;
		}

		Edge e(pVertex0,pVertex1);
		eIterator found = m_Edges.find(e);
		if (found == m_Edges.end()) m_Edges.insert(e);
		else m_Edges.erase(found);

	}
	const Vertex * vertex;
	edgeSet &m_Edges;
};

class VertexCircunCircleParallel {
public:
	VertexCircunCircleParallel(const Vertex * v,const triangleSet & tInput, triangleSet & tOutput, edgeSet & eOutput,int n) : m_Vertex(v), m_Triangles(tInput), o_Triangles(tOutput), o_Edges(eOutput), n_threads(n) {}

	void operator () (int thread) {
		ctIterator it = m_Triangles.begin() + (thread * m_Triangles.size() / n_threads);
		ctIterator itEnd = m_Triangles.begin() + ((thread + 1) * m_Triangles.size() / n_threads);
		vertexIsInCC vic(m_Vertex,o_Edges);

		for(; it != itEnd;it++) {
			mutex.lock();
			if (!vic(*it)) {
				o_Triangles.push_back(*it);
			}
			mutex.unlock();
		}
	}
protected:
	const Vertex * m_Vertex;
	const triangleSet & m_Triangles;
	triangleSet &o_Triangles;
	edgeSet &o_Edges;
	int n_threads;
	std::mutex mutex;

};

class InsertFinalTriangles {
public:
	InsertFinalTriangles(const Triangle & sT,const Quadtree & tree, triangleSet & output) : sTriangle(sT), quadtree(tree), triangles(output) {}
	void operator() (const Triangle & t) {
		for (int i = 0; i < 3; i++) {
			const Vertex * p = t.getVertex(i);
			for (int j = 0; j < 3; j++) {
				if (p == sTriangle.getVertex(j))
					return;
			}
		}
		triangles.push_back(t);
	}
protected:
	const Triangle & sTriangle;
	const Quadtree & quadtree;
	triangleSet & triangles;
};

void Delaunay::Triangulate(const vertexSet & vertices, triangleSet & output) {

	try {
		if (vertices.size() < 3)
			throw;

		cvIterator itVertex = vertices.begin();
		Vertex * v = *itVertex;
		float xMin = v->GetX();
		float yMin = v->GetY();
		float xMax = xMin;
		float yMax = yMin;

		++itVertex;
		for (; itVertex != vertices.end(); itVertex++) {
			v = *itVertex;
			xMin = (v->GetX() < xMin ? v->GetX() : xMin);
			xMax = (v->GetX() > xMax ? v->GetX() : xMax);
			yMin = (v->GetY() < yMin ? v->GetY() : yMin);
			yMax = (v->GetY() > yMax ? v->GetY() : yMax);
		}
		float dx = xMax - xMin;
		float dy = yMax - yMin;

		// Make the bounding box slightly bigger, just to feel safe.
		float ddx = dx * 0.01f;
		float ddy = dy * 0.01f;

		xMin -= ddx;
		xMax += ddx;
		dx += 2 * ddx;

		yMin -= ddy;
		yMax += ddy;
		dy += 2 * ddy;
		int lastId = vertices.size();
		Vertex * p0 = new Vertex(sf::Vector2f(xMin - dy * sqrt3 / 3.0f, yMin),lastId+1);
		Vertex * p1 = new Vertex(sf::Vector2f(xMax + dy * sqrt3 / 3.0f, yMin),lastId+2);
		Vertex * p2 = new Vertex(sf::Vector2f((xMin + xMax) * 0.5F, yMax + dx * sqrt3 * 0.5f),lastId+3);
		Triangle sTriangle(p0,p1,p2);

		triangleSet mesh;
		triangleSet aux;
		mesh.push_back(sTriangle);
		output.clear();
		Quadtree * tree = new Quadtree( 0.0f, 0.0f, 1024.0f, 768.0f, 0, 4);

		for(itVertex = vertices.begin(); itVertex != vertices.end(); itVertex++) {
			v = *itVertex;
			//tree->AddObject(v);

			edgeSet edges;
			vertexIsInCC vic(v,edges);
			aux.clear();
			/**
			 * Verifica todos triangulos que possuem o ponto circumscrito e salva as edges para reconstruir os triangulos
			 * outros triangulos sao inseridos em uma variavel auxiliar para adicionar posteriormente
			 */
			//paralelismo, verificar os triangulos paralelamente, inserindo na lista (concorrente :/ ) os edges
			int n_threads = 2;
			std::vector<std::thread> lThreads;
			VertexCircunCircleParallel obj (std::ref(v),std::ref(mesh),std::ref(aux),std::ref(edges),n_threads);
			for (int k = 0; k < n_threads; k++) {
//				obj(k);
				lThreads.push_back(std::thread(std::ref(obj),k));
			}
			for (auto& th : lThreads)
				th.join();

			mesh.clear(); // limpa o mesh para renovar os triangulos
			for(eIterator itEdges = edges.begin(); itEdges != edges.end(); itEdges++) {
				Triangle nTriangle((Vertex *)itEdges->m_pV0, (Vertex *)itEdges->m_pV1,v);
				mesh.push_back(nTriangle);
			}
			//insere os triangulos que nao foram modificados
			for(tIterator itAux = aux.begin(); itAux != aux.end();itAux++) {
//				//verificacao se as edges do triangulo modificado nao colidem com outras edges jah inseridas (era pra ser uma otimizacao, mas fica mais lento :v)
				//mas acho que sei pq, eh q agora nao tem mais o problema q tinha antes, dele inserir triangulos que se sobrepoem
//				Edge e01 = Edge(itAux->getVertex(0),itAux->getVertex(1));
//				Edge e12 = Edge(itAux->getVertex(1),itAux->getVertex(2));
//				Edge e20 = Edge(itAux->getVertex(2),itAux->getVertex(0));
//				int edgesIguais = 0;
//				for(eIterator itEdges = edges.begin(); itEdges != edges.end(); itEdges++) {
//					if (*itEdges == e01 || *itEdges == e12 || *itEdges == e20) {
//						edgesIguais++;
//					}
//				}
//				//se mais de 2 edges forem iguais, nao adiciona o triangulo
//				if (edgesIguais < 2) {
//					mesh.insert(*itAux);
//				}
//
				mesh.push_back(*itAux);
			}
		}
		//output = mesh;
		for_each(mesh.begin(),mesh.end(),InsertFinalTriangles(sTriangle,*tree,output));

	} catch (int e) {
		return;
	}
}
