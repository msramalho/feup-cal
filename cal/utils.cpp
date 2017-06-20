#include "utils.h"


double getTotalDistance(vector<Node> nodes) {
	double dist = 0;
	if (nodes.size() == 0)
		return dist;
	for (size_t i = 0; i < nodes.size() - 1; i++) {
		dist += nodes[i].distance(nodes[i + 1]);
	}
	return dist;
}

int randBetween(int min, int max) {
	return min + (rand() % (int)(max - min + 1));
}
int randDiff(int min, int max, vector<int> dif) {//get random number different from dif
	int res = randBetween(min, max);
	while (find(dif.begin(), dif.end(), res) != dif.end())
		res = randBetween(min, max);
	return res;
}


void cinValueStr(string &result, string message) {
	result = "";
	cin.ignore(1000, '\n');
	while (result == "") {
		cout << message;
		char cstr_result[1000];
		cin.getline(cstr_result, 1000, '\n');
		result = string(cstr_result);
		if (cin.fail()) {
			cin.clear();
		}
	}
	cin.clear();
}

