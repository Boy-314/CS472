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
	vector<object *> children;
};

// create root of state space
object * createObject(string n, double v, double w)
{
	object * temp = new object;
	temp->name = n;
	temp->value = v;
	temp->weight = w;
	return temp;
}

// preorder traversal
void printPreOrder(struct object * node)
{
	if(node == NULL)
	{
		return;
	}
	
	cout << node->name << " ";
	for(int i = 0; i < (node->children).size(); i++)
	{
		printPreOrder((node->children)[i]);
	}
}

/*
									   A                         B                 C           D            E
				  AB              AC      AD   AE        BC      BD   BE        CD   CE        DE
		ABC       ABD   ABE   ACD   ACE   ADE          BCD BCE   BDE            CDE
	ABCD   ABCE   ABDE        ACDE
	ABCDE
*/

// DFS with specified depth and target value
object * DFS(vector<object> knapsack, int depth, object * previous, double targetValue, double maximumWeight)
{
	if(previous == NULL)
	{
		return NULL;
	}
	
	if(previous->value >= targetValue && previous->weight <= maximumWeight)
	{
		cout << previous->name << ":" << previous->value << "," << previous->weight << endl;
		return previous;
	}
	
	if(depth == 0)
	{
		return NULL;
	}
	
	for(int i = 0; i < knapsack.size(); i++)
	{
		object * temp = createObject(previous->name + knapsack[i].name, previous->value + knapsack[i].value, previous->weight + knapsack[i].weight);
		(previous->children).push_back(temp);
		DFS(knapsack,depth - 1,temp,targetValue - temp->value,maximumWeight - temp->weight);
	}
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
	
	// iterate through depths
	for(int i = 1; i <= knapsack.size(); i++)
	{
		object * solution = DFS(knapsack,i,createObject("",0,0),targetValue,maximumWeight);
		if(solution != NULL)
		{
			cout << solution->name << endl;
			break;
		}
	}
	return 0;
}