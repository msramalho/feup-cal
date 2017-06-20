#include "OSMparser.h"


vector<string> readFile(string filename) {
	cout << "Reading file ("<< filename << ")...";
	ifstream f;
	string line;
	vector <string> result;

	f.open(filename);

	if (f.is_open()) {
		while (getline(f, line)) {
			if (line != "") {//in case there are any additional empty lines in the file
				result.push_back(line);
			}
		}
		f.close();
	}
	cout << "Done" << endl;
	return result;
}

bool fileExists(string filename) {
	ifstream f;
	f.open(filename);
	if (f.is_open()) {
		f.close();
		return true;
	} else {
		return false;
	}
}

Node getNodeFromFile1(string line) { //Node_id; latDeg; lonDeg; latRad; lonRad
	smatch matches;
	regex r("(\\d+);(.+);(.+);(.+);(.+)");

	if (regex_search(line, matches, r) && matches.size() == 6)
		return Node(matches[1], stod(matches[5]), stod(matches[4]));
	cout << "Failed to read NODE, wrong format(" << line << ")" << endl;

	return nullptr;
}

Road * getRoadFromFile2(string line) {//Road_id; StreetName; IsBothWays
	smatch matches;
	regex r("(\\d*);(.*);(False|True)");

	if (regex_search(line, matches, r) && matches.size() == 4)
		return new Road(matches[1], matches[2], matches[3] == "True");
	cout << "Failed to read ROAD, wrong format(" << line << ")" << endl;
	 
	return nullptr;
}

void getInfoFromFile3(string line, string & roadId, string & node1Id, string & node2Id) { //Road_id; Node1_id; Node2_id
	smatch matches;
	regex r("(\\d*);(\\d*);(\\d*)");

	if (regex_search(line, matches, r) && matches.size() == 4) {
		roadId = matches[1];
		node1Id = matches[2];
		node2Id = matches[3];
		return;
	}
	cout << "Failed to read EDGE, wrong format(" << line << ")" << endl;
}

Graph<Node>* getGraphFromFiles(string f1, string f2, string f3, vector<Road *> & roads) {
	vector<string> fc1, fc2, fc3;//file contents for each file
	roads.clear();//to hold the roads information

	Graph<Node> * graph =  new Graph<Node>;//the graph read from the files

	//check no mistakes
	if (!fileExists(f1) || !fileExists(f2) || !fileExists(f3)) {
		cout << "not all files exist ("<< f1 <<","<< f2<<"," << f3<< ")" << endl;
		return nullptr;
	}

	//read files into vectors
	fc1 = readFile(f1);
	fc2 = readFile(f2);
	fc3 = readFile(f3);

	//read all the nodes into the graph, after this loop there are only nodes
	cout << "Reading Nodes into the Graph...";
	for (auto line: fc1) 
		graph->addVertex(getNodeFromFile1(line));
	cout << "Done(" << graph->getNumVertex() << " Nodes)" << endl;

	//read all the rows into a vector of Road (passed by reference), to relate them to the nodes in the next section
	cout << "Reading Roads into vector...";
	for (auto line : fc2)
		roads.push_back(getRoadFromFile2(line));
	cout << "Done(" << roads.size() <<" roads)" << endl;

	//(string f1line, string & roadId, string & node1Id, string & node2Id)
	string rId, n1Id, n2Id; //variables to hold the:roadId, node1Id, node2Id
	Road * road;//the current row to be treated as a graph edge
	Node tempO = Node();
	Node tempD = Node();
	int countFailedRoads = 0, countEdges = 0;
	cout << "Creating the edges with the roads..." << endl;
	for (auto line : fc3) {
		countEdges++;
		getInfoFromFile3(line, rId, n1Id, n2Id);//read one connection type
		if ((road = Road::findRoadById(roads, rId)) == nullptr) {
			cout << "	Road with id " << rId << " not found" << endl;
			countFailedRoads++;
			continue;
		}
		tempO = Node(n1Id);//the nodes are initialized with the correct id so that the search works, 
		tempD = Node(n2Id);//because operator== is defined for the Node class
		if (!graph->addEdge(graph->getTwoVertexs(tempO, tempD), 0, road->bothWays))//adds the edge to the graph
			cout << "failed to add edge from " << n1Id << " to " << n2Id << endl;
	}
	cout << "...Done(failed " << countFailedRoads << "/"<< countEdges <<")" << endl;

	return graph;
}
