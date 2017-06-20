#pragma once

#include <cmath>
#include <string>
#include "Configs.h"

using namespace std;//should not be done her, but this is an independent project

#define PI 3.14159265358979323846
#define earthRadiusKm 6371.0

/**
* @brief Class Node used to represent an element of the graph
* This class holds the gps latitude and longitude elements that will be inherited by most of the classes that represent a vertex of the graph
*/
class Node {
protected:
	/**
	*double variables that specify the gps location in Degrees
	*/
	double lat, lon;	//radian values
	/**
	* Unique identifier for this Node
	*/
	string id;
public:
	/**
	* Empty constructor: id, lat and lon will be 0
	*/
	Node();
	/**
	* Constructor 2 to intialize the node with  id and gps coordinates
	* @param id the id of the node
	*/
	Node(string id);
	/**
	* Constructor to intialize the node with  id and gps coordinates
	* @param id the id of the node
	* @param lat the latitude of the point in the map
	* @param lon the longitude of the point in the map
	*/
	Node(string id, double lat, double lon);
	/**
	* getter for latitude
	* @return double containing the latitude value
	*/
	double getLat();
	/**
	* getter for longitude
	* @return double containing the longitude value
	*/
	double getLon();
	/**
	* getter for id
	* @return unsigned int (string) containing the id value
	*/
	string getId();

	/**
	* setter for latitude
	* @param lat double containing the latitude value
	*/
	Node & setLat(double lat);
	/**
	* setter for longitude
	* @param lon double containing the longitude value
	*/
	Node & setLon(double lon);
	/**
	* setter for id
	* @param id unsigned int (string) containing the id value
	*/
	Node & setId(string id);

	/**
	* Returns the distance between two points on the Earth.
	* Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
	* @param Node the node to compare to the current instance
	* @return The distance between the two Nodes in meters
	*/
	double distance(Node end);


	bool operator==(const Node & n) const;

	bool operator!=(const Node & n) const;
	/**
	* Output operator for a Node object
	* @param os the ostream to output to
	* @param right the Node to output
	* @return ostream reference to the passed ostream
	*/
	friend ostream& operator<<(ostream& os, const Node& n);
};