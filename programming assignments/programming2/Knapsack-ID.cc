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
	
	/*
	                        A                         B                C              D                   E
	        AB          AC     AD   AE        BC      BD   BE          CD   CE        DE
		ABC ABD ABE  ACD ACE   ADE          BCD BCE   BDE              CDE
	*/
	
	// construct state space
	object * root = createObject("",0,0);
	string previousName = "";
	for(int i = 0; i < knapsack.size(); i++)
	{
		for(int j = i; j < knapsack.size(); j++)
		{
			(root->children).push_back(createObject(knapsack[j].name,knapsack[j].value,knapsack[j].weight));
		}
	}
	
	// preorder traversal
	printPreOrder(root);
	
	return 0;
}