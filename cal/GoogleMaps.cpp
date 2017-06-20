#include "GoogleMaps.h"


string getStringFromFile(string filename) {
	ifstream t(filename);
	return string((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
}

void replace(string & str, string & from, string & to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}


void createMapFromFile(string templateFileName, vector<vector<Node>> & paths, vector<Node> & vertexes,
	Node garage, Node station, vector<string> containerIds, bool show) {
	string contents = getStringFromFile(templateFileName);
	string from, to;

	//the garage id
	from = "<|!TEXT TO BE REPLACED GARAGE_ID!|>";
	to = garage.getId();
	replace(contents, from, to);

	//the station id
	from = "<|!TEXT TO BE REPLACED SATION_ID!|>";
	to = station.getId();
	replace(contents, from, to);

	//the container ids
	from = "<|!TEXT TO BE REPLACED CONTAINER_IDS!|>";
	to = getListOfStrings(containerIds);
	replace(contents, from, to);

	//the markers in the map
	from = "<|!TEXT TO BE REPLACED MARKERS!|>";
	to = getCoordinatesMapsApi(vertexes);
	replace(contents, from, to);

	//the line points in the map
	from = "<|!TEXT TO BE REPLACED PATHS!|>";
	to = getCoordinatesArray(paths);
	replace(contents, from, to);

	ofstream out("output/maps.html");
	out << contents;
	out.close();
	if (show) {
		ShellExecute(NULL, _T("open"),
			_T("file:///C:/Users/M/Desktop/Development/Visual%20Studio/C++/CAL/cal/output/maps.html"), NULL, NULL, SW_SHOWNORMAL);
	}
}
string getCoordinatesMapsApi(vector<Node> & nodes) {
	stringstream ss;
	if (nodes.size() == 0)
		return "";

	for (size_t i = 0; i < nodes.size() - 1; i++)
		ss << "				{lat: toDegrees("<< nodes[i].getLat() << "), lng: toDegrees(" << nodes[i].getLon() << "),id: "<< nodes[i].getId() <<"}," << endl;

	ss << "				{lat: toDegrees(" << nodes[nodes.size() - 1].getLat() << 
		"), lng: toDegrees(" << nodes[nodes.size() - 1].getLon() << "),id: " << nodes[nodes.size() - 1].getId() << "}" << endl;
	return ss.str();
}

string getArrayFromStrings(vector<string> & items) {
	stringstream ss;
	if (items.size() == 0)
		return "";

	for (size_t i = 0; i < items.size() - 1; i++)
		ss << "				["<<endl << items[i] <<"				]," << endl;

	ss << "				[" << endl << items[items.size()-1] << "				]" << endl;
	return ss.str();
}

string getCoordinatesArray(vector<vector<Node>> & paths) {
	vector<string> items;
	for (auto path : paths)
		items.push_back(getCoordinatesMapsApi(path));
	return getArrayFromStrings(items);
}

string getListOfStrings(vector<string> source) {
	stringstream ss;
	if (source.size() == 0)
		return "";

	for (size_t i = 0; i < source.size() - 1; i++)
		ss  << source[i] << ", ";


	ss  << source[source.size() - 1];
	return ss.str();
}
