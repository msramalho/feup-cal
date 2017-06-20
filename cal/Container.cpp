#include "Container.h"

Container::Container():Node() {}

Container::Container(string id, float lat, float lon):Node(id, lat, lon) {}

Container::Container(string id, float lat, float lon, bool full, float volume, char type) 
	:Node(id, lat, lon), full(full), volume(volume), type(type) {}

Container::Container(Node n, bool full, float volume, char type):full(full), volume(volume), type(type) {
	this->id = n.getId();
	this->lat = n.getLat();
	this->lon = n.getLon();
}

bool Container::isFull() {return full;}

Container & Container::setFull(bool full) { this->full = full; return *this; }

float Container::getVolume() {return volume;}

Container & Container::setVolume(float volume) { this->volume = volume; return *this; }

char Container::getType() const { return type; }

Container & Container::setType(char type) { this->type = type; return *this; }

Node Container::getNode() {
	return Node(id, lat, lon);
}
