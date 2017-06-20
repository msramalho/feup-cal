#include <iostream>
#include <random>
#include <time.h>
#include <list>
#include <chrono>
#include <iomanip>

//First part
#include "OSMparser.h"
#include "GoogleMaps.h"
#include "Graph.h"
#include "Node.h"
#include "Road.h"
#include "CompleteGraph.h"
#include "utils.h"
//Second part
#include "StringMatching.h"

using namespace std;

chrono::time_point<chrono::system_clock> startTime;
chrono::duration<double> timeDiff;

void startRecordingTime() {
	startTime = chrono::system_clock::now();
}
void stopRecordingTime(size_t countNodes, string name) {
	timeDiff = chrono::system_clock::now() - startTime;
	cout << "TIME: " << countNodes << " " << timeDiff.count() << "(" << name << ")" << endl;
}


bool graphConnectivity(list <Container> cont, Graph<Node> *gr, Node initial);

/**
* Best path from the garage to the station, by one container
*/
vector<Node> singleTruckSingleContainer(CompleteGraph * cgraph, Truck * t, Container * c, Station * s) {
	vector<Node> res;
	vector<Vertex<Node> *> temp = cgraph->graph->getVertexSet();
	if (temp.size() <= 3) {
		cout << "Not enough nodes to run singleTruckSingleContainer" << endl;
		return res;//makes no sense to run the algorithm
	}
	//save the truck and container and station
	cgraph->trucks.clear();
	cgraph->addTruck(t);
	cgraph->containers.clear();
	cgraph->addContainer(c);
	cgraph->stations.clear();
	cgraph->addStation(s);
	//output the data
	cout << "Garage: " << t->getGarage().getId()
		<< ", Container: " << c->getId()
		<< ", Station: " << s->getId() << endl;
	//empty the vector and add the new satation

	//get the best path for this case
	vector<Vertex<Node> *> fullRes = cgraph->generateSingleTruckSingleContainer(t, c);
	fullRes = cgraph->backPreProcess(fullRes);
	res = Graph<Node>::getInfoFromVector(fullRes);
	for (auto node : res)
		cout << "->" << node.getId();
	cout << endl;
	cout << "TOTAL DISTANCE: " << getTotalDistance(res) << endl;
	return res;
}
vector<Node> randomSingleTruckSingleContainer(CompleteGraph * cgraph) {
	vector<Vertex<Node> *> temp = cgraph->graph->getVertexSet();

	//randomly choose three nodes
	int truckRand, containerRand, stationRand;
	truckRand = randBetween(0, temp.size() - 1);
	containerRand = randDiff(0, temp.size() - 1, { truckRand });
	stationRand = randDiff(0, temp.size() - 1, { truckRand, containerRand });


	Truck * t = new Truck(temp[truckRand]->getInfo(), 0, 'i');
	Container * c = new Container(temp[containerRand]->getInfo(), true, 0, 'i');//full container at a random pos
	Station * s = new Station(temp[stationRand]->getInfo());

	return singleTruckSingleContainer(cgraph, t, c, s);
}

/**
* one truck and multiple containers loaded into the containers vector, one station
*/
vector<Node> oneTruckMultipleContainers(CompleteGraph * cgraph, Truck * t, Station * s) {
	vector<Node> res;

	cout << "Garage: " << t->getGarage().getId()
		<< ", Containers: " << cgraph->containers.size()
		<< ", Station: " << s->getId() << endl;
	//Step 02 -> Nearest Neighbours - using distance matrix from cgraph
	//get the best path for this case
	vector<Container *> containersCopy = cgraph->containers;
	vector<Vertex<Node> *> fullRes = cgraph->generateOneTruckMultipleContainers(t, s);
	cgraph->containers = containersCopy;
	fullRes = cgraph->backPreProcess(fullRes);
	res = Graph<Node>::getInfoFromVector(fullRes);
	for (auto node : res)
		cout << "->" << node.getId();
	cout << endl;
	cout << "TOTAL DISTANCE: " << getTotalDistance(res) << endl;
	return res;
}
vector<Node> randomOneTruckMultipleContainers(CompleteGraph * cgraph, int countContainers) {
	if (cgraph->graph->getNumVertex() <= countContainers + 2) {
		cout << "Not enough nodes to run oneTruckMultipleContainers with" << countContainers << " containers" << endl;
		return vector<Node>();//makes no sense to run the algorithm
	}

	//randomly choose two nodes
	int truckRand, stationRand;
	vector<int> diffs;//not to repeat random numbers
	truckRand = randBetween(0, cgraph->graph->getNumVertex() - 1);
	diffs.push_back(truckRand);
	stationRand = randDiff(0, cgraph->graph->getNumVertex() - 1, diffs);
	diffs.push_back(stationRand);

	Truck * t = new Truck((*cgraph->graph)(truckRand)->getInfo(), 0, 'i');
	Station * s = new Station((*cgraph->graph)(stationRand)->getInfo());
	//save the truck and station
	cgraph->trucks.clear();
	cgraph->addTruck(t);
	cgraph->stations.clear();
	cgraph->addStation(s);

	//Step 01 -> N random containers
	cgraph->containers.clear();
	int n = randDiff(0, cgraph->graph->getNumVertex() - 1, diffs);
	diffs.push_back(n);
	for (int i = 0; i < countContainers; i++) {
		n = randDiff(0, cgraph->graph->getNumVertex() - 1, diffs);
		diffs.push_back(n);
		cout << "Container: " << (*cgraph->graph)(n)->getInfo() << endl;
		cgraph->addContainer(new Container((*cgraph->graph)(n)->getInfo(), true, 0, 'i'));//full container at a random pos
	}
	startRecordingTime();
	auto temp = oneTruckMultipleContainers(cgraph, t, s);
	stopRecordingTime(cgraph->graph->getNumVertex(), "oneTruckMultipleContainers/NearestNeighbour");
	return temp;
}

/**
* multiple trucks and multiple containers loaded into the containers vector, one station
*/
vector<vector<Node>> multipleTrucskMultipleContainers(CompleteGraph * cgraph, Truck * t, Station * s) {
	vector<vector<Node>> res;

	cout << "Garage: " << t->getGarage().getId()
		<< ", Containers: " << cgraph->containers.size()
		<< ", Station: " << s->getId() << endl;
	//Step 02 -> Nearest Neighbours - using distance matrix from cgraph
	//get the best path for this case
	vector<Container *> containersCopy = cgraph->containers;
	vector<vector<Vertex<Node>*>> fullRes = cgraph->generateMultipleTrucskMultipleContainers(s, 10);
	cgraph->containers = containersCopy;
	for (auto & truckPath : fullRes)
		truckPath = cgraph->backPreProcess(truckPath);
	for (auto truckPath : fullRes)
		res.push_back(Graph<Node>::getInfoFromVector(truckPath));

	//output paths and stats
	double totalDist = 0;
	for (auto path : res) {
		totalDist += getTotalDistance(path);
		for (auto node : path)
			cout << "->" << node.getId();
		cout << endl;
	}
	cout << "TOTAL DISTANCE: " << totalDist << endl;
	cout << "TOTAL TRUCKS:   " << res.size() << endl;
	return res;
}
vector<vector<Node>> randomMultipleTrucksMultipleContainers(CompleteGraph * cgraph, int countContainers, float containerSizes) {
	if (cgraph->graph->getNumVertex() <= countContainers + 2) {
		cout << "Not enough nodes to run randomMultipleTrucksMultipleContainers with " << countContainers << " containers" << endl;
		return vector<vector<Node>>();//makes no sense to run the algorithm
	}

	//randomly choose two nodes
	int truckRand, stationRand;
	vector<int> diffs;//not to repeat random numbers
	truckRand = randBetween(0, cgraph->graph->getNumVertex() - 1);
	diffs.push_back(truckRand);
	stationRand = randDiff(0, cgraph->graph->getNumVertex() - 1, diffs);
	diffs.push_back(stationRand);


	/*truckRand = 5;
	stationRand = 6;*/

	Truck * t = new Truck((*cgraph->graph)(truckRand)->getInfo(), 10, 'i');
	Station * s = new Station((*cgraph->graph)(stationRand)->getInfo());
	//save the truck and station
	cgraph->trucks.clear();
	cgraph->addTruck(t);
	cgraph->stations.clear();
	cgraph->addStation(s);

	//Step 01 -> N random containers
	cgraph->containers.clear();
	int n = randDiff(0, cgraph->graph->getNumVertex() - 1, diffs);
	diffs.push_back(n);
	for (int i = 0; i < countContainers; i++) {
		n = randDiff(0, cgraph->graph->getNumVertex() - 1, diffs);
		diffs.push_back(n);
		cout << "Container: " << (*cgraph->graph)(n)->getInfo() << endl;
		cgraph->addContainer(new Container((*cgraph->graph)(n)->getInfo(), true, containerSizes, 'i'));//full container at a random pos
	}

	return multipleTrucskMultipleContainers(cgraph, t, s);
}

/**
*	This function will execute an exact string search on the street names
*/
void exactStreetSearch(CompleteGraph * cgraph) {
	string street1, street2;//user input street names
	vector<int> smatches1, smatches2;//street matches 1 and 2
	cinValueStr(street1, "What's the first street name: ");
	smatches1 = exactSearch(cgraph->roads, street1);
	if (smatches1.size() == 0) {
		cout << "Street 1 name does not have an exact match... exiting exact search" << endl;
		return;
	}
	cout << smatches1.size() << " matches found for '" << street1 << "'" << endl;

	//Second street name
	cinValueStr(street2, "What's the second street name: ");
	smatches2 = exactSearch(cgraph->roads, street2);
	if (smatches2.size() == 0) {
		cout << "Street 2 name does not have an exact match... exiting exact search" << endl;
		return;
	}
	cout << smatches2.size() << " matches found for '" << street2 << "'" << endl;
	int countIntersections = 0;
	for (int i = 0; i < smatches1.size(); i++) {
		for (int j = 0; j < smatches2.size(); j++) {
			if (cgraph->roadsIntersection[smatches1[i]][smatches2[j]]) {//if this is a valid intersection
				cout << "Intersection between ids: " << setw(12) << cgraph->roads[smatches1[i]]->id << " and " <<
					setw(12) << cgraph->roads[smatches2[j]]->id << endl;
				countIntersections++;
			}
		}
	}
	cout << "Total of " << countIntersections << " intersections between exact matches of "<< street1 << " and " << street2 << endl;

}

struct less_than_pair{
	inline bool operator() (pair<int, pair<int, Road*>> p1, pair<int, pair<int, Road*>> p2)	{
		return (p1.first < p2.first);
	}
};

/**
* Returns the index of the road in the cgraph->roads
*/
pair<int, pair<int, Road*>> getApproximateStreetString(CompleteGraph * cgraph) {
	//vector<pair<int, Road *>> streetMatches(cgraph->roads.size(), make_pair<int, Road*>(0, NULL));//for each road there is a pair: index, proximityToSearch value to street1
	vector<pair<int, pair<int, Road * >>> streetMatches(cgraph->roads.size(), make_pair<int, pair<int, Road * >>(0, make_pair<int, Road*>(0, NULL)));//for each road there is a pair: proximityToSearch value to street1, index in cgraph->roads, Road*
	//pair<editDistance, pair<indexIn cgraph->roads, Road * >>

	string street;//user input street names
	cinValueStr(street, "What's the street name: ");

	startRecordingTime();
	for (int i = 0; i < cgraph->roads.size(); i++) {
		streetMatches[i].first = editDistance(street, cgraph->roads[i]->name);
		streetMatches[i].second.first = i;
		streetMatches[i].second. second = cgraph->roads[i];
	}
	stopRecordingTime(cgraph->roads.size(), "getApproximateStreetString");


	sort(streetMatches.begin(), streetMatches.end(), less_than_pair());//sort by proximity

	cout << "How many streets to display (max " << cgraph->roads.size() << "): ";
	int countDisplay;
	cin >> countDisplay;
	countDisplay = min(countDisplay, cgraph->roads.size());
	for (int i = 0; i < countDisplay; i++) {
		cout << "number" << setw(3)<< (i+1) <<"name: " << setw(40) << streetMatches[i].second.second->name << ", id: " << setw(14) << streetMatches[i].second.second->id << ", proximity: " << streetMatches[i].first << endl;
	}
	int choosenNumber = 0;
	do {
		cout << "Choose the number associated with the street you were referring to: " << endl;
		cin >> choosenNumber;
	} while (choosenNumber <= 0 || choosenNumber > countDisplay);
	
	return streetMatches[choosenNumber - 1];
}

/**
*	This function will execute an approximate string search on the street names
*/
void approximateStreetSearch(CompleteGraph * cgraph) {
	//pair<editDistance, pair<indexIn cgraph->roads, Road * >>
	pair<int, pair<int, Road*>> street1, street2;

	cout << "Street 1 Name: " << endl;
	street1 = getApproximateStreetString(cgraph);
	cout << "Street 2 Name: " << endl;
	street2 = getApproximateStreetString(cgraph);
	if (cgraph->roadsIntersection[street1.second.first][street2.second.first]) //if this is a valid intersection
		cout << "Intersection between ids: " << setw(12) << street1.second.second->id << " and " << setw(12) << street2.second.second->id << endl<< "Street names: " << street1.second.second->name << " and " << street2.second.second->name << endl;
	else
		cout << "No intersection found for this two streets" << endl;
}
int main() {
	srand(time(NULL));//randomizer
	cout << "Usage: " << endl;
	cout << "All Nodes: a" << endl;
	cout << "The other programed functions: any other" << endl;
	char c;
	cin >> c;

	CompleteGraph * cgraph = new CompleteGraph("osmData/small_01.txt", "osmData/small_02.txt", "osmData/small_03.txt");
	//CompleteGraph * cgraph = new CompleteGraph("osmData/hardcoded_01.txt", "osmData/hardcoded_02.txt", "osmData/hardcoded_03.txt");
	//CompleteGraph * cgraph = new CompleteGraph("osmData/large_01.txt", "osmData/large_02.txt", "osmData/large_03.txt");
	//CompleteGraph * cgraph = new CompleteGraph("osmData/01_01.txt", "osmData/01_02.txt", "osmData/01_03.txt");
	//CompleteGraph * cgraph = new CompleteGraph("osmData/02_01.txt", "osmData/02_02.txt", "osmData/02_03.txt");
	//CompleteGraph * cgraph = new CompleteGraph("osmData/03_01.txt", "osmData/03_02.txt", "osmData/03_03.txt");
	//CompleteGraph * cgraph = new CompleteGraph("osmData/04_01.txt", "osmData/04_02.txt", "osmData/04_03.txt");

	startRecordingTime();
	cgraph->preProcessGraph();
	stopRecordingTime(cgraph->graphOriginal->getNumVertex(), "preProcessGraph");

	vector<Vertex<Node> *> temp = cgraph->graphOriginal->getVertexSet();
	cout << "Calculating paths..." << endl;
	//paths.push_back(graph.graphOriginal->getPath(temp[0]->getInfo(), temp[5]->getInfo()));
	cout << "...Done" << endl;

	temp = cgraph->graph->getVertexSet();
	vector<Node> vertexes;
	if (c == 'a') {
		for (auto node : temp)
			vertexes.push_back(node->getInfo());
	}
	cout << "1 if you want to test the first part of the program\n2 if you want to test the second part" << endl;
	int part;
	cin >> part;

	string command = "";
	if (part == 1) {//Path calculation
		startRecordingTime();
		cgraph->floydWarshallShortestPath();
		stopRecordingTime(cgraph->graph->getNumVertex(), "floydWarshallShortestPath");

		cout << "size temp = " << temp.size() << endl;

		do {
			cin.clear();
			vector<vector<Node>> paths;
			cout << "COMMAND: " << endl;
			cout << "   1              -> One truck  One Container" << endl;
			cout << "   2 (containers) -> One truck  N   Containers" << endl;
			cout << "   3 (containers) -> N   trucks N   Containers" << endl;
			cout << "   4              -> Check graph connectivity" << endl;
			cout << "   Else           -> Stop the program" << endl;
			cin >> command;
			int countContainers;

			if (command == "1")
				paths.push_back(randomSingleTruckSingleContainer(cgraph));
			else if (command == "2") {
				cin >> countContainers;
				paths.push_back(randomOneTruckMultipleContainers(cgraph, countContainers));
			} else if (command == "3") {
				cin >> countContainers;
				paths = randomMultipleTrucksMultipleContainers(cgraph, countContainers, 5);
			} else if (command == "4") {
				list<Container> cont;
				for (auto c : cgraph->containers)
					cont.push_back(*c);
				graphConnectivity(cont, cgraph->graph, cgraph->trucks[0]->getGarage());
				cin >> countContainers;
				paths = randomMultipleTrucksMultipleContainers(cgraph, countContainers, 5);
			} else
				break;
			if (paths.size() > 0)
				createMapFromFile("output/googleMapsTemplate.html", paths, vertexes, cgraph->trucks[0]->getGarage(),
					cgraph->stations[0]->getNode(), cgraph->getContainerIds(), true);
			else
				cout << "NO LEGAL PATH FOUND, due to incomplete OSM data" << endl;
		} while (true);
	} else if (part == 2) {
		cout << "There are " << cgraph->roads.size() << " roads, how many random intersections to generate?" << endl;
		int countRandIntersections;
		cin >> countRandIntersections;
		cout << "Generating " << countRandIntersections << " random intersections...";
		cgraph->randomizeRoads(countRandIntersections);
		cout << "Done" << endl;

		do {
			cin.clear();
			cout << "COMMAND: " << endl;
			cout << "   1     -> Exact Search" << endl;
			cout << "   2     -> Approximate Search" << endl;
			cout << "   3     -> Display roads information" << endl;
			cout << "   Else  -> Stop the program" << endl;
			cin >> command;

			if (command == "1"){
				cout << "Exact Search:" << endl;
				exactStreetSearch(cgraph);
			} else if (command == "2") {
				cout << "Approximate Search:" << endl;
				approximateStreetSearch(cgraph);
			} else if (command == "3") {
				cgraph->displayRoads();
			} else
				break;
			
		} while (true);
	}



	cout << "DONE" << endl;
	//paths.push_back(Graph<Node>::getInfoFromVector(cgraph->graph->getfloydWarshallPath(temp[7], temp[3])));//example of shortest path
	//paths.push_back(randomSingleTruckSingleContainer(cgraph));
	//paths.push_back(randomOneTruckMultipleContainers(cgraph, 4));
	//paths = randomMultipleTrucksMultipleContainers(cgraph, 5, 5);
	/*createMapFromFile("output/googleMapsTemplate.html", paths, vertexes, cgraph->trucks[0]->getGarage(),
	cgraph->stations[0]->getNode(), cgraph->getContainerIds(), true);*/


	/*vector<Road *> roads;
	Graph<Node> * graph;
	graph = getGraphFromFiles("osmData/small_01.txt", "osmData/small_02.txt", "osmData/small_03.txt", roads);*/

	/*for (auto road : roads)
	cout << (*road) << endl;*/

	/*vector<Vertex<Node *> *> temp = graph->getVertexSet();
	for (auto vertex : temp)
	cout << vertex->getInfo()->getId() << endl;*/

	//hide irrelevant
	/*graph->preProcessGraph();
	cout << "Checking redundant..." << endl;
	for (auto vertex : graph->getVertexSet())
	if(vertex->in.size() == 1 && vertex->adj.size() == 1 && !vertex->hidden)
	cout << vertex->getInfo() << " -> In:" << vertex->in.size() << ", out" << vertex->adj.size() << endl;
	cout << "...Done"<<endl;

	vector<Vertex<Node> *> temp = graph->getVertexSet();
	vector<vector<Node>> paths;
	cout << "Calculating paths..." << endl;
	paths.push_back(graph->getPath(temp[0]->getInfo(), temp[13]->getInfo()));
	paths.push_back(graph->getPath(temp[30]->getInfo(), temp[35]->getInfo()));
	paths.push_back(graph->getPath(temp[40]->getInfo(), temp[45]->getInfo()));
	paths.push_back(graph->getPath(temp[36]->getInfo(), temp[584]->getInfo()));
	cout << "...Done" << endl;

	vector<Node> vertexes;
	if (c == 'a') {
	for (auto node : temp)
	vertexes.push_back(node->getInfo());
	paths.clear();
	}*/


	//createMapFromFile("googleMapsTemplate.html", paths, vertexes, true);

	return 0;
}



bool graphConnectivity(list <Container> cont, Graph<Node> *gr, Node initial) {
	vector<Node> connected = gr->bfs(gr->getVertex(initial));
	gr->resetVisited();

	cout << "\nSee if connectivity of graph is satisfied?" << endl;

	for (list<Container>::iterator it = cont.begin(); it != cont.end(); it++) {// for every container
		for (unsigned int i = 0; i <= connected.size(); i++) {// for every node that we can acess from initial
			if (connected[i] == (*it).getNode()) { // verifies
				break;
			}
			if (i == connected.size()) {
				stringstream ss;
				string id;

				ss << (*it).getNode().getId();
				id = ss.str();

				cout << "Connectivity error: Cointainer" + id + " cannot be reached!" << endl;

				getchar();
				return false;
			}
		}
		for (list<Container>::iterator ita = cont.begin(); ita != cont.end(); ita++) {
			if (it != ita) {
				vector<Node> connected2 = gr->bfs(gr->getVertex((*it).getNode()));
				gr->resetVisited();

				for (unsigned int i = 0; i <= connected2.size(); i++) {
					if (connected2[i] == (*ita).getNode())
						break;
					if (i == connected2.size()) {
						stringstream ss1, ss2;
						string id_s, id_d;

						ss1 << (*it).getNode().getId();
						id_s = ss1.str();

						ss2 << (*ita).getNode().getId();
						id_d = ss2.str();

						cout << "Connectity error: Ecoponto " + id_d + "cannot be reached from " + id_s + "!" << endl;

						return false;
					}
				}
			}
		}
	}
	cout << "Finished connectivity test." << endl;

	return true;
}