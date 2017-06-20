#pragma once

#include "Node.h"

class Truck{
	/**
	* Node garage where starts its trip
	*/
	Node garage;
	/**
	* float volume of trash able to carry
	*/
	float volume;
	/**
	* char type of trash transported by truck
	*/
	char type;
public:
	/**
	* Empty constructor
	*/
	Truck();
	/**
	* Constructor to intialize the truck with grage, volume and type
	* @param node garagem assigned to Truck
	* @param volume of the truck
	* @param type of trash
	*/
	Truck(Node garage, float volume, char type = 'i');
	/**
	* getter of the Garage of the current node
	* @return reference to the Garage node of the current Truck
	*/
	Node getGarage();
	/**
	* set a garage to Truck
	* @param Node garagem to be assigned to Truck
	* @return reference to the current Truck
	*/
	Truck & setGarage(Node garage);
	/**
	* getter of Truck's volume
	* @return float volume value 
	*/
	float getVolume();
	/**
	* set a new volume to truck
	* @param new float volume value 
	* @return reference to the current Truck
	*/
	Truck & setVolume(float volume);
	/**
	* volume adder
	* @param foat value added to volume
	* @return reference to the current Truck
	*/
	Truck & addVolume(float value);
	/**
	* getter of Truck's trash type.
	* @return type of trash
	*/
	char getType()const;
	/**
	* set new type of trash carried by Truck
	* @param new type of trash 
	* @return reference to the current Truck
	*/
	Truck & setType(char type);
};