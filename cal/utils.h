#pragma once

#include<vector>
#include <iostream>
#include "Node.h"

using namespace std;


/**
* Read string from the keyboard through std::cin.getline(), after outputing a message related to the
* value to read so as to allow strings with word-breaks.
* @param message the message to show to user while asking for a non-empty string
* Returns:
* @param result read string value
*/
void cinValueStr(string &result, string message);


double getTotalDistance(vector<Node> nodes);

int randBetween(int min, int max);
int randDiff(int min, int max, vector<int> dif);

