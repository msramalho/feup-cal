#pragma once

#include "Node.h"

class Station: public Node{
protected:
	/**
	* type of waste treated
	*/
	char type;
public:
	/**
	* Empty constructor: id, lat and lon will be 0
	*/
	Station();
	/**
	* Constructor to intialize the Station with a Node
	* @param Node to be assinged
	*/
	Station(Node n);
	/**
	* Constructor to intialize the Station with id and gps coordinates
	* @param id the id of the Station node
	* @param lat the latitude of the point in the map
	* @param lon the longitude of the point in the map
	*/
	Station(string id, float lat, float lon);
	/**
	* Constructor to intialize the node with  id and gps coordinates
	* @param id the id of the Station node
	* @param lat the latitude of the point in the map
	* @param lon the longitude of the point in the map
	* @param type of the residual treated by the Station
	*/
	Station(string id, float lat, float lon, char t);
	/**
	* getter for longitude
	* @return char containing the type of waste treated by Station	
	*/
	char getType()const;
	/**
	* getter for longitude
	* @param type of waste that the Station will treat
	* @return reference to the current Station node
	*/
	Station & setType(char t);
	/**
	* getter of The station's node
	* @return Node assigned to station
	*/
	Node getNode();
};