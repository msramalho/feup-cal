#include "Truck.h"

Truck::Truck(){}

Truck::Truck(Node garage, float volume, char type):garage(garage),volume(volume),type(type) {}

Node Truck::getGarage() { return garage; }

Truck & Truck::setGarage(Node garage) { this->garage = garage; return *this; }

float Truck::getVolume() { return volume; }

Truck & Truck::setVolume(float volume) { this->volume = volume; return *this; }

Truck & Truck::addVolume(float value) {	this->volume += value; return *this;}

char Truck::getType() const { return type; }

Truck & Truck::setType(char type) { this->type = type; return *this; }
