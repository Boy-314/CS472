#include <algorithm>
#include <bits/stdc++.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
using namespace std;

/*
Iterative Deepening for Solving Knapsack Problem
*/

/*
struct for each object in the knapsack
name: object name (alphabetic character string)
value
weight
*/
struct object
{
	string name;
	double value;
	double weight;
};

int main()
{
	double targetValue, maximumWeight;
	
	// vector containing all objects in the knapsack
	vector<object> knapsack;
	
	// variables for each object
	string name;
	double value, weight;
	
	// read the file and process input into knapsack vector
	ifstream inputFile("input.txt");
	inputFile >> targetValue >> maximumWeight;
	while(inputFile >> name >> value >> weight)
	{
		object item = {name,value,weight};
		knapsack.push_back(item);
	}
	inputFile.close();
	
	return 0;
}