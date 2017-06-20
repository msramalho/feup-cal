#pragma once

#include <string>
#include <vector>
#include <ostream>

using namespace std;

class Road {
public:
	/**
	* string o the road id
	*/
	string id;
	/**
	* string name of the road
	*/
	string name;
	/**
	* bool value that tells if the road has two ways
	*/
	bool bothWays;
	/**
	* Empty constructor: id will be 0, name will be "NotNamed" and bothWays will be false
	*/
	Road();
	/**
	* Constructor to intialize the road with id, name and bothWays
	* @param id the id of the road node
	* @param name the name of the road
	* @param bothWays the param if the road has two ways or not
	*/
	Road(string id, string name, bool bothWays);
	/**
	* Output operator for a Road object
	* @param os the ostream to output to
	* @param right the Road to output
	* @return ostream reference to the passed ostream
	*/
	friend ostream& operator<<(ostream& os, const Road& r);
	/**
	* road finder
	* @param vector of pointer to roads
	* @param string id of node to be searched
	* @return pointer to road reference with the id searched
	*/
	static Road * findRoadById(vector<Road *> roads, string id);
};

