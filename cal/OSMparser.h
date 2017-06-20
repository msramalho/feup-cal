#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>

#include "Graph.h"
#include "Node.h"
#include "Road.h"

using namespace std;


/**
* Reads all the lines from a file into a vector<string> (empty if no file exists)
* @param filename of the file to be read
* @return vector <string> with all the lines
*/
vector<string> readFile(string filename);

/**
* Tests if a filename corresponds to a valid file
* @param filename to search
* @return boolean true if the file exists/ is readable or false if it does not
*/
bool fileExists(string filename);

/**
* Creates a node from a line with the format:Node_id; latDeg; lonDeg; latRad; lonRad, from the first file
* @param string line of info for node creation
* @return node pointer to the node created
*/
Node getNodeFromFile1(string line);

/**
* Creates a road from a line with the format: Road_id; StreetName; IsBothWays
* @param string line of info for node creation
* @return node pointer to the node created
*/
Road* getRoadFromFile2(string line);

/**
* Gets the values of a line like Road_id; Node1_id; Node2_id into variables
* @param string line of info for node creation
* @param string roadId of info for road creation
* @param string node1Id of info for node1
* @param string node2Id of info for node2
*/
void getInfoFromFile3(string line, string & roadId, string & node1Id, string & node2Id);



/**
* Create a graph from the output of openstreet maps
* @param f1, file 1, format: Node_id; latDeg; lonDeg; latRad; lonRad
* @param f2, file 2, format: Road_id; StreetName; IsBothWays
* @param f3, file 3, format: Road_id; Node1_id; Node2_id
*/
Graph<Node> * getGraphFromFiles(string f1, string f2, string f3, vector<Road *> & roads);

