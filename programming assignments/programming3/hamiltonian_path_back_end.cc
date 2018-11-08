#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdlib.h>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

int main()
{
	ifstream davis_putnam_output_file;
	davis_putnam_output_file.open("DavisPutnamOutput.txt");
	vector<string> truth_values;
	vector<string> variables;
	string line;
	while(getline(davis_putnam_output_file,line))
	{
		istringstream iss(line);
		int index;
		string value;
		iss >> index >> value;
		if(index == 0)
		{
			break;
		}
		truth_values.push_back(value);
	}
	while(getline(davis_putnam_output_file,line))
	{
		istringstream iss(line);
		int index;
		string variable;
		iss >> index >> variable;
		variables.push_back(variable);
	}
	davis_putnam_output_file.close();
	
	ofstream back_end_output_file;
	back_end_output_file.open("BackEndOutput.txt");
	for(int i = 0; i < truth_values.size(); i++)
	{
		if(truth_values[i] == "T")
		{
			back_end_output_file << variables[i] << " ";
		}
	}
	back_end_output_file.close();
	return 0;
}