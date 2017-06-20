#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "Road.h"

using namespace std;

void pre_kmp(string pattern, vector<int> & prefix);

int kmp(string text, string pattern);

int numStringMatching(string filename, string toSearch);

int editDistance(string pattern, string text);

float numApproximateStringMatching(string filename, string toSearch);
/**
*	Returns the indexes of the roads that are an exact match for the string to search, it will be empty if no match was found
*/
vector<int> exactSearch(vector<Road *> roads, string toSearch);

