#include "Road.h"



Road::Road():Road("0","NotNamed",false){}

Road::Road(string id, string name, bool bothWays):id(id), bothWays(bothWays) {
	if (name == "")
		this->name = "NotNamed";
	else
		this->name = name;
}

Road * Road::findRoadById(vector<Road *> roads, string id) {
	for (auto road : roads)
		if (road->id == id)
			return road;
	return nullptr;
}

ostream & operator<<(ostream & os, const Road & r) {
	os << "{id: "<< r.id << ", name: " << r.name << ", bothWays: " << (r.bothWays ? "True" : "False")  << "}";
	return os;
}
