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
	vector<char> name;
	double value;
	double weight;
	vector<object *> children;
};

// create root of state space
object * createObject(vector<char> n, double v, double w)
{
	object * temp = new object;
	temp->name = n;
	temp->value = v;
	temp->weight = w;
	return temp;
}

/*
									   A                         B                 C           D            E
				  AB              AC      AD   AE        BC      BD   BE        CD   CE        DE
		ABC       ABD   ABE   ACD   ACE   ADE          BCD BCE   BDE            CDE
	ABCD   ABCE   ABDE        ACDE
	ABCDE
*/

// check if a state is a goal
bool goal(object * o, double targetValue, double maximumWeight)
{
	if(o->value => targetValue && o->weight <= maximumWeight)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// DFS with specified depth
object * DFS(object * start, int depth)
{
	
}


// iterative deepening
object * IDS(object * start, int maxDepth)
{
	
}

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
	
	// run iterative deepening
	
	return 0;
}