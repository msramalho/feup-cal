#include "Station.h"
#include "Node.h"

using namespace std;

Station::Station():Node(), type('a') { }

Station::Station(Node n) {
	id = n.getId();
	lat = n.getLat();
	lon = n.getLon();
}

Station::Station(string id, float lat, float lon):Node(id, lat, lon) {}

Station::Station(string id, float lat, float lon, char t){}

char Station::getType()const {return type;}

Station & Station::setType(char t){
	this->type = t;
	return *this;
}

Node Station::getNode() {
	return Node(id, lat, lon);
}
