#include "CompleteGraph.h"



CompleteGraph::CompleteGraph() {}

CompleteGraph::CompleteGraph(string f1, string f2, string f3) {
	graphOriginal = getGraphFromFiles(f1, f2, f3, roads);
}

CompleteGraph & CompleteGraph::displayRoads(){
	cout << roads.size() << " roads:" << endl;
	for (auto road : roads)
		cout << *road << endl;
	return *this;
}

CompleteGraph & CompleteGraph::randomizeRoads(int nIntersections){
	//intialize the matrix of roads by roads that shows the roads intersection
	roadsIntersection = vector<vector<bool>>(roads.size(), vector<bool>(roads.size(), false));
	for (int i = 0; i < nIntersections; i++) {
		roadsIntersection[randBetween(0, roads.size() - 1)][randBetween(0, roads.size() - 1)] = true;
	}

	return *this;
}

CompleteGraph & CompleteGraph::preProcessGraph() {
	graph = graphOriginal->preProcessGraph();
	return *this;
}

bool CompleteGraph::addTruck(Truck * t) {
	if (find(trucks.begin(), trucks.end(), t) == trucks.end()) {//not in the vector, so it will be added
		trucks.push_back(t);
		return true;
	}
	return false;
}

bool CompleteGraph::addStation(Station * s) {
	if (find(stations.begin(), stations.end(), s) == stations.end()) {//not in the vector, so it will be added
		stations.push_back(s);
		return true;
	}
	return false;
}

bool CompleteGraph::addContainer(Container * c) {
	if (find(containers.begin(), containers.end(), c) == containers.end()) {//not in the vector, so it will be added
		containers.push_back(c);
		return true;
	}
	return false;
}

vector<string> CompleteGraph::getContainerIds() {
	vector<string> res;
	for (auto c : containers)
		res.push_back(c->getId());
	return res;
}

vector<Vertex<Node> * > CompleteGraph::generateSingleTruckSingleContainer(Truck * t, Container * c) {
	vector<Vertex<Node> * > result, temp;
	//assumes that floydWarshallShortestPath was called once before this function
	//and that there is only one station
	if (stations.size() != 1) {
		cout << "To run generateSingleTruckSingleContainer there can only be one station" << endl;
		exit(1);
	}


	//first identify the position of the truck and the container
	pair<Vertex<Node>*, Vertex<Node>*> nodes = graph->getTwoVertexs(t->getGarage(), c->getNode());
	Vertex<Node> * station = graph->getVertex(stations[0]->getNode());

	result = graph->getfloydWarshallPath(nodes.first, nodes.second);//shortest from garage to container
	temp = graph->getfloydWarshallPath(nodes.second, station);//shortest from container to station
	result.insert(result.end(), temp.begin() + 1, temp.end());//concatenate the results
	result.push_back(station);
	return result;
}

vector<Vertex<Node>*> CompleteGraph::generateOneTruckMultipleContainers(Truck * t, Station * s) {
	vector<Vertex<Node> * > result, temp;
	//assumes that floydWarshallShortestPath was called once before this function
	//the containers will be deleted and restored at the end
	//vector<Container *> containersCopy = containers;

	//first identify the position of the truck and the station
	pair<Vertex<Node>*, Vertex<Node>*> nodes = graph->getTwoVertexs(t->getGarage(), s->getNode());
	Vertex<Node> * garage = nodes.first, * station = nodes.second;
	int currentVertex = garage->posAtVec;

	//cout << "Initial Node: " << (*graph)(currentVertex)->getInfo().getId() << "("<< t->getGarage().getId() <<")" <<endl;
	
	//recalculates the distance from the garage to all the containers and sort the containers vector ASC
	recalculateDistContainers(currentVertex, true);
	
	Vertex<Node> * current = (*graph)(currentVertex), * next = (*graph)(containers[0]->posAtVecContainer);
	result = graph->getfloydWarshallPath(current, next);//shortest from garage to container
	while (containers.size() > 0 && t->getVolume() >= containers[0]->getVolume()) {
		//cout << "sending container: " << containers[0]->getNode() << "-> " << containers[0]->getVolume() << endl;
		t->addVolume(-containers[0]->getVolume());
		cout << "added container(1): "; 
		cout << containers[0]->getNode() << endl;
		containers.erase(containers.begin());//erase the first
		if (containers.size() == 0)
			break;
		recalculateDistContainers(current->posAtVec, true);//recalculates the distances and sorts again
		cout << "CONTAINERS: " << containers.size()  << endl;
		current = next;
		next = (*graph)(containers[0]->posAtVecContainer);
		temp = graph->getfloydWarshallPath(current, next);//shortest from container to container
		for (size_t i = 0; i < containers.size(); i++) {//iterate the vertexes in the new path and check for containers passed
			if (temp.size() > 0 
				&& find(temp.begin(), temp.end(), (*graph)(containers[i]->posAtVecContainer)) != temp.end()
				&& t->getVolume() >= containers[i]->getVolume()) {//if the container is in the new path
					t->addVolume(-containers[i]->getVolume());
					cout << "added container: ";
					cout << containers[i]->getNode() << endl;
					containers.erase(containers.begin()+i);
					cout << "ERASED: " << i << endl;
					i--;
			} 
		}
		result.insert(result.end(), temp.begin() + 1, temp.end());//concatenate the results
	}

	temp = graph->getfloydWarshallPath(next, station);//shortest from container to container
	result.insert(result.end(), temp.begin() + 1, temp.end());//concatenate the results
	result.push_back(station);//concatenate the results

	//containers = containersCopy;//restore the destroyed containers vector

	return result;
}

vector<vector<Vertex<Node>*>> CompleteGraph::generateMultipleTrucskMultipleContainers(Station * s, double trucksVolume) {
	vector<vector<Vertex<Node>*>> result;
	//int randTruck = randBetween(0, graph->getNumVertex() - 1);
	Truck * t = trucks[0];
	while (containers.size() > 0) {
		cout << "TRUCK" << containers.size() << endl;
		t->setVolume(trucksVolume);
		result.push_back(generateOneTruckMultipleContainers(t, s));
	}
	//todo deal with last container

	return result;
}

void CompleteGraph::floydWarshallShortestPath() {
	cout << "Starting Floyd Warshall...";
	graph->floydWarshallShortestPath();
	cout << "Done" << endl;
}


vector<Vertex<Node>*> CompleteGraph::backPreProcess(vector<Vertex<Node>*> shortenedPath) {
	vector<Vertex<Node>*> finalPath;
	Vertex<Node> * finalVertex = shortenedPath[shortenedPath.size() - 1];

	for (size_t i = 0; i < shortenedPath.size() - 1; i++) {//percorre o caminho encurtado
		finalPath.push_back(shortenedPath[i]);
		for (auto edge: shortenedPath[i]->adj) {//para cada edge do vertex atual
			if (edge.dest == shortenedPath[i + 1])//se o edge for dar ao proximo do encurtado
				finalPath.insert(finalPath.end(), edge.hidden.begin(), edge.hidden.end());//desenrola o caminho encurtado e mostra os escondidos entre esses dois
		}
	}
	//finalPath.push_back(shortenedPath[shortenedPath.size() - 1]);//adiciona o ultimo
	while (finalPath.size()>0 && finalPath[finalPath.size() - 1] != finalVertex) {
		finalPath.pop_back();
	}
	return finalPath;
}

void CompleteGraph::recalculateDistContainers(int currentVertexPos, bool isSort) {
	Vertex<Node> * tempVertex;
	for (auto container : containers) {
		//cout << "-------DIST: " << container->getNode() << endl;
		tempVertex = graph->getVertex(container->getNode());
		container->posAtVecContainer = tempVertex->posAtVec;
		container->dist = graph->dist[currentVertexPos][container->posAtVecContainer];
	}
	//cout << "DIST DONE, SORTING..." << endl;
	if(isSort && containers.size() > 0)
		sort(containers.begin(), containers.end(), compareContainerPointers());//sorting the containers for the near neighbour algorithm
	cout << "SORT DONE" << endl;
}


