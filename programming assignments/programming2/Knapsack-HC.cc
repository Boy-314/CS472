#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;

/*
Hill Climbing with 10 random restarts for solving the Knapsack Problem
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

// method for calculating error for a given state
double findError(object * o, double targetValue, double maximumWeight)
{
	double error = max(o->weight - maximumWeight,0.0) + max(targetValue - o->value,0.0);
	return error;
}

// hill climbing
object * hillClimbing(vector<object> knapsack, object * o, double targetValue, double maximumWeight, int restart)
{
	double currentError = findError(o, targetValue, maximumWeight);
	double futureError = findError(o, targetValue, maximumWeight);
	vector<double> minError;
	if(currentError == 0)
	{
		return o;
	}
	object * current = o;
	while(currentError != 0)
	{
		// try adding an element to the current state
		for(int i = 0; i < knapsack.size(); i++)
		{
			// if element is already in the object
			bool exist = false;
			string element = knapsack[i].name[0];
			for(int j = 0; j < (o->name).size(); j++)
			{
				if(element == (o->name)[j])
				{
					exist = true;
				}
			}
			if(exist)
			{
				continue;
			}
			
			// otherwise try adding element, and store future error
			else if(!exist)
			{
				vector<string> newName = o->name;
				newName.push_back(element);
				object add = {newName, o->value + knapsack[i].value, o->weight + knapsack[i].weight};
				futureError = findError(&add, targetValue, maximumWeight);
				minError.push_back(futureError);
			}
		}
		
		// try deleting an element from the current state
		for(int i = 0; i < (o->name).size(); i++)
		{
			vector<string> newName = o->name;
			
			// find corresponding index in knapsack of the element we are deleting
			int index = 0;
			while(knapsack[index].name[0] != (o->name)[i])
			{
				index++;
			}
			
			newName.erase(newName.begin() + i);
			object remove = {newName, o->value - knapsack[index].value, o->weight - knapsack[index].weight};
			futureError = findError(&remove, targetValue, maximumWeight);
			minError.push_back(futureError);
		}
		
		// try swapping an element from inside the state from one not inside the state
		for(int i = 0; i < knapsack.size(); i++)
		{
			// find element not inside state
			bool exist = false;
			string element = knapsack[i].name[0];
			for(int j = 0; j < (o->name).size(); j++)
			{
				if(element == (o->name)[j])
				{
					exist = true;
				}
			}
			if(exist)
			{
				continue;
			}
			
			// otherwise try adding element, remove another element to emulate the "swap"
			else if(!exist)
			{
				for(int j = 0; j < (o->name).size(); j++)
				{
					vector<string> newName = o->name;
					int index = 0;
					while(knapsack[index].name[0] != (o->name)[j])
					{
						index++;
					}
					
					newName.erase(newName.begin() + j);
					newName.push_back(knapsack[i].name[0]);
					object swap = {newName, o->value - knapsack[index].value + knapsack[i].value, o->weight - knapsack[index].weight + knapsack[i].weight};
					futureError = findError(&swap, targetValue, maximumWeight);
					minError.push_back(futureError);
				}
			}
		}
		
		stable_sort(minError.begin(), minError.end());
		futureError = minError[0];
		// if no improvement in error, random restart
		if(futureError >= currentError && restart > 0)
		{
			int randIndex = rand() % knapsack.size();
			object randomRestart = {knapsack[randIndex].name, knapsack[randIndex].value, knapsack[randIndex].weight};
			currentError = findError(&randomRestart, targetValue, maximumWeight);
			restart--;
		}

		// otherwise update the error
		else if(futureError < currentError)
		{
			currentError = futureError;
		}
	}
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
	// REPLACE WITH YOUR TEST FILENAME HERE
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
	
	object * ans = hillClimbing(knapsack, &start, targetValue, maximumWeight, 10);
	if(ans != nullptr)
	{
		// cout << "final ans->name.size(): " << (ans->name).size() << endl << flush;
		for(int i = 0; i < (ans->name).size(); i++)
		{
			cout << "Output: " << (ans->name)[i] << " ";
		}
	}
	
	if(ans == nullptr)
	{
		cout << "Output: No solution";
	}
	
	return 0;
}