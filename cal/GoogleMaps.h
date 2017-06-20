#pragma once
/**
* This file will be used to output some graph edges onto a google maps map
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <Windows.h>
#include <tchar.h>

#include "Node.h"

using namespace std;


void createMapFromFile(string templateFileName, vector<vector<Node>> & paths, vector<Node> & vertexes,
	Node garage, Node station, vector<string> containerIds, bool show = false);

string getStringFromFile(string filename);


void replace(string& str, string& from, string& to);

string getCoordinatesMapsApi(vector<Node> & nodes);

string getArrayFromStrings(vector<string> & items);

string getCoordinatesArray(vector<vector<Node>> & paths);

string getListOfStrings(vector<string> source);