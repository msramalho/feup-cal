#include "Node.h"

Node::Node():Node("",0,0) {}

Node::Node(string id, double lat, double lon):id(id),lat(lat),lon(lon) {}

Node::Node(string id):id(id) {}

double Node::getLat() {return lat;}

double Node::getLon() {return lon;}

string Node::getId() {return id;}

Node & Node::setLat(double lat) { this->lat = lat; return *this; }

Node & Node::setLon(double lon) { this->lon = lon; return *this;}

Node & Node::setId(string id) {this->id = id; return *this;}

double Node::distance(Node end) {
	double u, v;
	u = sin((end.lat - lat) / 2);
	v = sin((end.lon - lon) / 2);
	return 1000 * 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat) * cos(end.lat) * v * v));//returns the distance in meters
}

bool Node::operator==(const Node & n) const {
	return id == n.id;
}

bool Node::operator!=(const Node & n) const {
	return !this->operator==(n);
}

ostream & operator<<(ostream & os, const Node & n) {
	os << "{id: " << n.id << ", lat: " << n.lat << ", lon: " << n.lon << "}";
	return os;
}
