#pragma once

#include "Node.h"

class Container : public Node{
	/**
	* boolean value that determines if the container is full or not
	* true for full
	* false for not full
	*/
	bool full;
	/**
	* float value of the container volume
	*/
	float volume;
	/**
	* type of waste dumped into the container
	*/
	char type;
public:
	/**
	* Distance from this container to the nearest station 
	*/
	double dist;
	/**
	* Index of the container in the vector of containers
	*/
	int posAtVecContainer;
	/**
	* Empty constructor: id, lat and lon will be 0
	*/
	Container();
	/**
	* Constructor to intialize the container with id and gps coordinates
	* @param id the id of the Container node
	* @param lat the latitude of the point in the map
	* @param lon the longitude of the point in the map
	*/
	Container(string id, float lat, float lon);
	/**
	* Constructor to intialize the node with id and gps coordinates full value, volume and type
	* @param id the id of the Container node
	* @param lat the latitude of the point in the map
	* @param lon the longitude of the point in the map
	* @param full the starting value telling if the container is full or not
	* @param volume of the container
	* @param type of the residual dumped into the container
	*/
	Container(string id, float lat, float lon, bool full, float volume, char type);
	/**
	* Constructor to intialize the node with id of node in the parameters, full value, volume and type
	* @param n the node that will be a Container
	* @param full the starting value telling if the container is full or not
	* @param volume of the container
	* @param type of the residual dumped into the container
	*/
	Container(Node n, bool full, float volume, char type);
	/**
	* determiner of container emptyness
	* @return bool value telling if the container is full or not
	*/
	bool isFull();
	/**
	* setter of container emptyness
	* @param bool value to be set container full or not
	* @return reference to the current container node
	*/
	Container & setFull(bool full);
	/**
	* setter of container emptyness
	* @return float value of the volume of container
	*/
	float getVolume();
	/**
	* setter of container volume
	* @param foat volume value to be set to container 
	* @return reference to the current container node
	*/
	Container & setVolume(float volume);
	/**
	* getter for longitude
	* @return char containing the type of waste the container holds
	*/
	char getType()const;
	/**
	* getter for longitude
	* @param type of waste that the Container will hold
	* @return reference to the current Container node
	*/
	Container & setType(char type);

	/**
	* getter of the current Container node
	* @return current Container node
	*/
	Node getNode();
};
struct compareContainerPointers {
	/**
	* operator comparing two Containers
	* @param  lhs pointer to Container 
	* @param  rhs pointer to Container
	* @return true if the right Container has greater distance
	*/
	bool const operator()(Container *lhs, Container *rhs) const {
		return lhs->dist < rhs->dist;
	}
};