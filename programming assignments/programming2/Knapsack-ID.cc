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

// DFS with specified depth
object * DFS(vector<object> knapsack, int depth, string previous)
{
	object * root = createObject("",0,0);
	int index = 0;
	for(int i = 0; i < depth; i++)
	{
		(root->children).push_back(createObject(previous + knapsack[index].name,knapsack[index].value,knapsack[index].weight));
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
	                                       A                         B                 C           D            E
	                  AB              AC      AD   AE        BC      BD   BE        CD   CE        DE
		    ABC       ABD   ABE   ACD   ACE   ADE          BCD BCE   BDE            CDE
		ABCD   ABCE   ABDE        ACDE
		ABCDE
	*/
	
	// iterate through depths
	for(int i = 1; i <= knapsack.size(); i++)
	{
		DFS(knapsack,i,"");
	}
	return 0;
}