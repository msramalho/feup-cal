#pragma once

#include <vector>
#include <queue>
#include <climits>

using namespace std;

#define INFINITE_INT INT_MAX

template <class T> class Edge;
template <class T> class Graph;

//---------------------------------------Vertex
template <class T>
class Vertex {
	T info;
	//int in;//the Vertexes that go into this vertex
	void addEdge(Vertex<T> *dest, double w);
	int dist;
public:
	bool visited;
	vector<Edge<T>  > adj;
	vector<Vertex<T>* > in;//the Vertexes that go into this vertex
	Vertex<T> * path;
	Vertex(T info);
	T getInfo();
	friend class Graph<T>;
	bool removeEdgeTo(Vertex<T> *d);
	bool hidden;//if this vertex is transversable
	size_t posAtVec;//saves the position of the vertex in the graph's vertexSet
	void setVisited(bool v);
};

template <class T>
void Vertex<T>::setVisited(bool v) {
	visited = v;
}

template <class T>
Vertex<T>::Vertex(T info) : info(info), visited(false), dist(0), path(nullptr), hidden(false){}

template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	for (auto i = adj.begin(); i != adj.end(); i++){//iterate adjacent of current
		if (i->dest = d) {//if one of the adjacent is 
			for (vector<Vertex<T>* >::iterator vit = i->dest->in.begin(); vit != i->dest->in.end(); vit++) {//remove from the "in"
				if ((*vit) == this) {
					i->dest->in.erase(vit);
					adj.erase(i);//only delete it afterwards
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

template<class T>
T Vertex<T>::getInfo() {
	return info;
}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {
	Edge<T> edgeD(dest, this->info.distance(dest->info));//requires the template to have a distance method, this->info.distance(dest->info)
	//edgeD.dest->in++;// updates the number of vertexes going into the destination Vertex
	edgeD.dest->in.push_back(this);//Adds this vertex to the vector of vertexes going into the new one
	adj.push_back(edgeD);
}





//---------------------------------------Edge
template <class T>
class Edge {
	double weight;
public:
	Vertex<T> * dest;
	vector<Vertex<T> *> hidden;//vector of Vertexes that have been culminated into this edge in the preprocessing part
	Edge(Vertex<T> *d, double w);
	static Edge<T> mergeEdges(Edge<T> left, Edge<T> right, Vertex<T> * removedVertex);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w){}

template<class T>
Edge<T> Edge<T>::mergeEdges(Edge<T> left, Edge<T> right, Vertex<T> * removedVertex) {
	Edge<T> res = Edge<T>(right.dest, left.weight + right.weight);
	//add the hidden vertexes by the correct order, from left to right
	for (auto v : left.hidden)
		res.hidden.push_back(v);
	res.hidden.push_back(removedVertex);
	for (auto v : right.hidden)
		res.hidden.push_back(v);
	return res;
}





//---------------------------------------Graph
template <class T>
class Graph {
	void dfs(Vertex<T> *v, vector<T> &res) const;
	vector<Vertex<T> *> vertexSet;
public:
	bool addVertex(const T &in);
	bool addVertexPointer(Vertex<T> * in);
	bool addEdge(Vertex<T> *vS, Vertex<T> *vD, double w, bool bothWays = false);//updated to receive vertex
	bool addEdge(pair<Vertex<T> *, Vertex<T> *> sourceAndDest, double w, bool bothWays = false);//updated to receive vertex pair
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(Vertex<T> *v) const;
	int maxNewChildren(Vertex<T> *v, T &inf) const;
	vector<Vertex<T> * > getVertexSet() const;
	int getNumVertex() const;

	void unweightedShortestPath(const T &v);
	vector<T> getPath(T source, T dest);
	Vertex<T> * getVertex(const T &sourc);//get a vertex from its content identifier
	pair<Vertex<T> *, Vertex<T> *> getTwoVertexs(const T &sourc, const T &dest);//get a vertex from its content identifier
	Graph<T> * preProcessGraph();//aggregates Vertexes that can be aggregated to otimize the other graph functions

	static vector<T> getInfoFromVector(vector<Vertex<T> * > source);
	//For Floyd Warshal:
	vector<vector<double>> dist;//weights
	vector<vector<int>> next;//to reconstruct the path after running the algorithm
	double edgeCost(int i, int j);
	int vertexPrev(int i, int j);
	void floydWarshallShortestPath();
	vector<Vertex<T> * > getfloydWarshallPath(Vertex<T> * u, Vertex<T> * v);
	Vertex<T> * operator()(int n);

	void resetVisited();
};

template<class T>
void Graph<T>::resetVisited() {
	for (unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->setVisited(false);
	}
}

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}
template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
	return vertexSet;
}

template <class T>
bool Graph<T>::addVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++)
		if ((*it)->info == in) return false;
	Vertex<T> *v1 = new Vertex<T>(in);
	v1->posAtVec = vertexSet.size();
	vertexSet.push_back(v1);
	return true;
}

template<class T>
inline bool Graph<T>::addVertexPointer(Vertex<T> * in) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	for (; it != ite; it++)
		if ((*it)->info == in->info) return false;
	in->posAtVec = vertexSet.size();
	vertexSet.push_back(in);
	return true;
}

template <class T>
bool Graph<T>::removeVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++) {
		if ((*it)->info == in) {
			Vertex<T> * v= *it;
			vertexSet.erase(it);
			typename vector<Vertex<T>*>::iterator it1= vertexSet.begin();
			typename vector<Vertex<T>*>::iterator it1e= vertexSet.end();
			for (; it1!=it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}
			delete v;
			return true;
		}
	}
	return false;
}

template <class T>
bool Graph<T>::addEdge(Vertex<T> *vS, Vertex<T> *vD, double w, bool bothWays = false) {
	/*typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS = nullptr, *vD = nullptr;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
			{ vS=*it; found++;}
		if ( (*it)->info == dest )
			{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;*/
	if (vS == nullptr || vD == nullptr) return false;
	vS->addEdge(vD,w);
	if (bothWays)
		vD->addEdge(vS, w);
	return true;
}

template<class T>
bool Graph<T>::addEdge(pair<Vertex<T>*, Vertex<T>*> sourceAndDest, double w, bool bothWays) {
	return addEdge(sourceAndDest.first, sourceAndDest.second, w, bothWays);
}

template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
			{ vS=*it; found++;}
		if ( (*it)->info == dest )
			{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;
	return vS->removeEdgeTo(vD);
}




template <class T>
vector<T> Graph<T>::dfs() const {
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	vector<T> res;
	it=vertexSet.begin();
	for (; it !=ite; it++)
	    if ( (*it)->visited==false )
	    	dfs(*it,res);
	return res;
}

template <class T>
void Graph<T>::dfs(Vertex<T> *v,vector<T> &res) const {
	v->visited = true;
	res.push_back(v->info);
	typename vector<Edge<T> >::iterator it= (v->adj).begin();
	typename vector<Edge<T> >::iterator ite= (v->adj).end();
	for (; it !=ite; it++)
	    if ( it->dest->visited == false )
	    	dfs(it->dest, res);
}

template <class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	q.push(v);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
			}
		}
	}
	return res;
}

template <class T>
int Graph<T>::maxNewChildren(Vertex<T> *v, T &inf) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	queue<int> level;
	int maxChildren=0;
	inf =v->info;
	q.push(v);
	level.push(0);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		int l=level.front();
		level.pop(); l++;
		int nChildren=0;
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
				level.push(l);
				nChildren++;
			}
		}
		if (nChildren>maxChildren) {
			maxChildren=nChildren;
			inf = v1->info;
		}
	}
	return maxChildren;
}




template <class T>
void Graph<T>::unweightedShortestPath(const T &vertex) {
	queue<Vertex<T> * > q;//queue to use the fifo

	Vertex<T> * vs = nullptr;
	for (auto v : vertexSet)
		if (v->info == vertex) {
			vs = v;
			break;
		}


	//set the distances to INFINITE_INT
	for (auto el : vertexSet)
		el->dist = INFINITE_INT;

	vs->dist = 0;//set the starting vertex distance to 0
	q.push(vs);
	Vertex<T> *v;
	while (q.size()>0) {
		v = q.front(); q.pop();
		for (auto edge : v->adj) {
			if (edge.dest->dist == INFINITE_INT) {
				edge.dest->dist = v->dist + 1;
				edge.dest->path = v;
				q.push(edge.dest);
			}
		}
	}
}



template <class T>
vector<T> Graph<T>::getPath(T source, T dest) {
	unweightedShortestPath(source);
	vector<T> result;
	Vertex<T> * vs = nullptr;
	Vertex<T> * vd = nullptr;
	for (auto v : vertexSet) {
		if (v->info == source)
			vs = v;
		else if (v->info == dest)
			vd = v;

		if (vs != nullptr && vd != nullptr)
			break;
	}

	if (vs == nullptr || vd == nullptr)
		return result;


	result.push_back(vd->info);
	while (vd->info != vs->info && vd->path != nullptr) {
		vd = vd->path;
		result.push_back(vd->info);
	}
	reverse(result.begin(), result.end());
	return result;
}

template<class T>
Vertex<T>* Graph<T>::getVertex(const T & source) {
	for (auto v : vertexSet)
		if (v->info == source)
			return v;
	return nullptr;
}

template<class T>
pair<Vertex<T>*, Vertex<T>*> Graph<T>::getTwoVertexs(const T & sourc, const T & dest) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	int found = 0;
	Vertex<T> *vS = nullptr, *vD = nullptr;
	while (found != 2 && it != ite) {
		if ((*it)->info == sourc) 
			{vS=*it; found++;}
		if ((*it)->info == dest)
			{vD=*it; found++;}
		it++;
	}
	if (found != 2)
		cout << "getTwoVertexes " << sourc << ", " << dest << " failed" << endl;
	return pair<Vertex<T>*, Vertex<T>*>(vS, vD);
}

template<class T>
Graph<T> * Graph<T>::preProcessGraph() {
	cout << "Preprocessing...";
	Graph<T> * resultGraph = new Graph<T>;
	for (auto v : vertexSet)
		v->hidden = false;

	int countRedundant = 0;
	int countRedundantNotFixed = 0;
	for (auto v : vertexSet) {
		if (v->adj.size() == 1 && v->in.size() == 1) {//this Vertex has only one edge coming in and one leaving
			//cout << "---->REDUNDANT NODE: " << v->getInfo().getId();
			countRedundantNotFixed++;
			Edge<T> enter = Edge<T>(nullptr, 0);
			bool found = false;
			vector<Edge<T>>::iterator eit;
			for (eit = v->in[0]->adj.begin(); eit != v->in[0]->adj.end(); eit++) {
				if (eit->dest == v) {
					//cout << "("<< (*eit).dest->getInfo().getId() <<")";
					found = true;
					enter = (*eit);
					break;
				}
			}
			if (found) {//this is just a failsafe
				//cout << " connects to " << v->adj[0].dest->getInfo().getId()<< ", which has the " 
					//<< v->adj[0].dest->in[0]->getInfo().getId() << " as entering and " << (v->adj[0].dest->in.size() - 1) << " more";
				Edge<T> sum = Edge<T>::mergeEdges(enter, v->adj[0], v);//sum the weight and the hidden of both edges in the right order
				v->in[0]->adj.erase(eit);//remove this vertex from the adjacent of the left vertex
				v->in[0]->adj.push_back(sum);//connect the left to the right
				for (auto i = v->adj[0].dest->in.begin(); i != v->adj[0].dest->in.end(); i++) {
					if ((*i)->getInfo() == v->getInfo()) {//find the in of the right that matches the vertex being removed to erase it
						v->adj[0].dest->in.erase(i);
						countRedundant++;
						//cout << "->FIXED";
						break;
					}
				}
				//cout << endl;
				v->adj[0].dest->in.push_back(v->in[0]);//push the left into the IN of the right
				v->hidden = true;
			}
		}
	}
	for (auto v : vertexSet)
		if (!v->hidden)
			resultGraph->addVertexPointer(v);
	cout << "Done(" << countRedundant << " Redundant Nodes, "<< countRedundantNotFixed  <<" Found)" << endl;
	return resultGraph;
}

template<class T>
vector<T> Graph<T>::getInfoFromVector(vector<Vertex<T>*> source) {
	vector<T> result;
	for (auto v : source)
		result.push_back(v->getInfo());
	return result;
}


template<class T>
double Graph<T>::edgeCost(int i, int j) {
	if (i == j)
		return 0;

	for (auto edge : vertexSet[i]->adj)
		if (edge.dest == vertexSet[j])
			return edge.weight;

	return INFINITE_INT;
}
template<class T>
int Graph<T>::vertexPrev(int i, int j) {
	for (auto edge : vertexSet[i]->adj) {
		if (edge.dest == vertexSet[j])
			return j;
	}

	return -1;
}


template<class T>
void Graph<T>::floydWarshallShortestPath() {
	int N = vertexSet.size(); //size of the square matrix
	dist = vector<vector<double> >(N, vector<double>(N));
	next = vector<vector<int> >(N, vector<int>(N));

	//first step, set weight and previous
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			dist[i][j] = edgeCost(i, j);
			next[i][j] = vertexPrev(i, j);
		}


	for (int k = 0; k < N; k++)// standard Floyd-Warshall implementation
		for (int j = 0; j < N; j++)
			for (int i = 0; i < N; i++)
				if (dist[i][j] > (dist[i][k] + dist[k][j])) {
					dist[i][j] = dist[i][k] + dist[k][j];
					next[i][j] = next[i][k];
				}

}

template<class T>
vector<Vertex<T> * > Graph<T>::getfloydWarshallPath(Vertex<T>* Vu, Vertex<T>* Vv) {
	vector<Vertex<T> * > result;
	int found = 0;
	int u = Vu->posAtVec, v = Vv->posAtVec;/* , i = 0;

	for (auto vertex : vertexSet) {
		if (vertex == Vu) { u = i;  found++; }
		else if (vertex == Vv) { v = i; found++; }
		if (found == 2)
			break;
		i++;
	}

	if (next[u][v] == -1 || found != 2)
		return result;*/

	result.push_back(vertexSet[u]);
	while (u != v) {
		u = next[u][v];
		if (u < 0)
			break;
		result.push_back(vertexSet[u]);
	}
	return result;
}

template<class T>
inline Vertex<T>* Graph<T>::operator()(int n) {
	return vertexSet[n];
}
