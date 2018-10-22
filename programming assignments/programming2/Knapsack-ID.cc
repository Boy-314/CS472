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
	vector<string> name;
	double value;
	double weight;
};

// create root of state space
object * createObject(vector<string> n, double v, double w)
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
	if(o->value >= targetValue && o->weight <= maximumWeight)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// DFS with specified depth
object * DFS(vector<object> knapsack, object * o, int depth, double targetValue, double maximumWeight)
{
	// if state is a goal state, return it
	if(goal(o, targetValue, maximumWeight))
	{
		return o;
	}

	// if depth is reached
	if(depth == 0)
	{
		return nullptr;
	}
	
	int lastIndex = 0;
	string last = "";
	if((o->name).size() != 0)
	{
		lastIndex = (o->name).size() - 1;
		last = (o->name)[lastIndex];
	}

	vector<object> children;
	for(int i = 0; i < knapsack.size(); i++)
	{
		// base case
		if(last == "")
		{
			if(o->weight + knapsack[i].weight <= maximumWeight)
			{
				vector<string> newName = o->name;
				newName.push_back((knapsack[i].name)[0]);
				cout << endl;
				for(int i = 0; i < newName.size(); i++)
				{
					cout << newName[i];
				}
				cout << endl;
				object child = {newName, o->value + knapsack[i].value, o->weight + knapsack[i].weight};
				object * ans = DFS(knapsack, &child, depth - 1, targetValue, maximumWeight);
				if(ans != nullptr)
				{
					return ans;
				}
			}
		}
		
		// check for all names that come alphabetically after the last element in the object's name
		else if(last.compare((knapsack[i].name)[0]) < 0)
		{
			// if adding the letter maintains the maximumWeight
			if(o->weight + knapsack[i].weight <= maximumWeight)
			{
				vector<string> newName = o->name;
				newName.push_back((knapsack[i].name)[0]);
				cout << endl;
				for(int i = 0; i < newName.size(); i++)
				{
					cout << newName[i];
				}
				cout << endl;
				object child = {newName, o->value + knapsack[i].value, o->weight + knapsack[i].weight};
				object * ans = DFS(knapsack, &child, depth - 1, targetValue, maximumWeight);
				if(ans != nullptr)
				{
					return ans;
				}
			}
		}
	}
	return nullptr;
}


// iterative deepening
object * IDS(vector<object> knapsack, object * start, int maxDepth, double targetValue, double maximumWeight)
{
	for(int i = 1; i <= maxDepth; i++)
	{
		object * ans = DFS(knapsack, start, i, targetValue, maximumWeight);
		if(ans != nullptr)
		{
			return ans;
		}
	}
	return nullptr;
}

vector<string> split(string str)
{
	vector<string> ret;
	for(int i = 0; i < str.length(); i++)
	{
		ret.push_back(str.substr(i,1));
	}
	return ret;
}

int main()
{
	double targetValue, maximumWeight;
	
	// vector containing all objects in the knapsack
	vector<object> knapsack;
	
	// variables for each object
	string nameString;
	double value, weight;
	
	// read the file and process input into knapsack vector
	ifstream inputFile("input.txt");
	inputFile >> targetValue >> maximumWeight;
	while(inputFile >> nameString >> value >> weight)
	{
		vector<string> name = split(nameString);
		object item = {name,value,weight};
		knapsack.push_back(item);
	}
	inputFile.close();
	
	int maxDepth = knapsack.size();
	
	// run iterative deepening
	vector<string> startingName;
	object start = {startingName, 0, 0};
	object * ans = IDS(knapsack, &start, maxDepth, targetValue, maximumWeight);
	
	if(ans != nullptr)
	{
		cout << "final ans->name.size(): " << (ans->name).size() << endl << flush;
		// for(int i = 0; i < (ans->name).size(); i++)
		// {
			// cout << "Output: " << (ans->name)[i] << " ";
		// }
	}
	
	if(ans == nullptr)
	{
		cout << "Output: No solution";
	}
	
	return 0;
}