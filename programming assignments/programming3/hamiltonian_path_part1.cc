#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <stdlib.h>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

struct edge
{
	string vertex_start;
	string vertex_end;
};

int main()
{
	int num_of_vertices;
	string vertex_start;
	string vertex_end;
	vector<edge> graph;
	vector<string> all_atoms;
	
	// open input file and store number of vertices and all the edges
	ifstream front_end_input_file("FrontEndInput.txt");
	front_end_input_file >> num_of_vertices;
	while(front_end_input_file >> vertex_start >> vertex_end)
	{
		edge temp = {vertex_start, vertex_end};
		graph.push_back(temp);
	}
	front_end_input_file.close();
	
	// prepare the output file
	ofstream front_end_output_file;
	front_end_output_file.open("FrontEndOutput.txt");
	
	// get a set of vertices
	set<string> vertices;
	for(int i = 0; i < graph.size(); i++)
	{
		vertices.insert(graph[i].vertex_start);
		vertices.insert(graph[i].vertex_end);
	}
	
	// compile a vector of all possible atoms
	for(auto i : vertices)
	{
		string atom;
		for(int time = 1; time <= num_of_vertices; time++)
		{
			atom = i;
			atom += to_string(time);
			all_atoms.push_back(atom);
		}
	}
	
	// rule: every vertex is traversed at some time
	for(auto i : vertices)
	{
		for(int j = 0; j < num_of_vertices; j++)
		{
			int index = 999999;
			string atom;
			atom = i;
			atom += to_string(j + 1);
			for(int k = 0; k < all_atoms.size(); k++)
			{
				if(all_atoms[k] == atom)
				{
					index = k + 1;
					break;
				}
			}
			front_end_output_file << index << " ";
		}
		front_end_output_file << endl;
	}
	
	// rule: no pair of vertices are traversed at the same time
	for(auto i : vertices)
	{
		for(auto j : vertices)
		{
			if(i == j)
			{
				continue;
			}
			for(int time = 1; time <= num_of_vertices; time++)
			{
				int index_one = 999999;
				int index_two = 999999;
				string atom_one;
				string atom_two;
				atom_one = i;
				atom_one += to_string(time);
				atom_two = j;
				atom_two += to_string(time);
				for(int k = 0; k < all_atoms.size(); k++)
				{
					if(all_atoms[k] == atom_one)
					{
						index_one = k + 1;
					}
					if(all_atoms[k] == atom_two)
					{
						index_two = k + 1;
					}
				}
				front_end_output_file << "-" << index_one << " -" << index_two << endl;
			}
		}
	}
	
	// rule: cannot go from edge at time t to edge at time t + 1 if there is no edge between the two
	// create a complete graph
	vector<edge> complete_graph;
	for(auto i : vertices)
	{
		for(auto j : vertices)
		{
			if(i == j)
			{
				continue;
			}
			edge temp = {i,j};
			complete_graph.push_back(temp);
		}
	}
	
	// create a graph that contains all edges that our graph does not contain
	vector<edge> missing_edges = complete_graph;
	for(int i = 0; i < graph.size(); i++)
	{
		int index = 0;
		while(missing_edges[index].vertex_start != graph[i].vertex_start || missing_edges[index].vertex_end != graph[i].vertex_end)
		{
			index++;
		}
		missing_edges.erase(missing_edges.begin() + index);
	}
	
	// create rule: cannot go from one edge at time t to another edge at time t + 1 if there is no edge from the first edge to the second
	for(int i = 0; i < missing_edges.size(); i++)
	{
		int time = 1;
		while(time + 1 <= num_of_vertices)
		{
			int index_one = 999999;
			int index_two = 999999;
			string atom_one;
			string atom_two;
			atom_one = missing_edges[i].vertex_start;
			atom_one += to_string(time);
			atom_two = missing_edges[i].vertex_end;
			atom_two += to_string(time + 1);
			for(int j = 0; j < all_atoms.size(); j++)
			{
				if(all_atoms[j] == atom_one)
				{
					index_one = j + 1;
				}
				if(all_atoms[j] == atom_two)
				{
					index_two = j + 1;
				}
			}
			front_end_output_file << "-" << index_one << " -" << index_two << endl;
			time++;
		}
	}
	
	// optional rule: at every time there is a vertex
	for(int time = 1; time <= num_of_vertices; time++)
	{
		for(auto i : vertices)
		{
			int index = 999999;
			string atom;
			atom = i;
			atom += to_string(time);
			for(int j = 0; j < all_atoms.size(); j++)
			{
				if(all_atoms[j] == atom)
				{
					index = j + 1;
				}
			}
			front_end_output_file << index << " ";
		}
		front_end_output_file << endl;
	}
	
	// optinoal rule: no vertex is traversed more than once
	for(auto i : vertices)
	{
		int time_one = 1;
		
		while(time_one <= num_of_vertices)
		{
			int time_two = 1;
			while(time_two <= num_of_vertices)
			{
				if(time_one == time_two)
				{
					time_two++;
					continue;
				}
				int index_one = 999999;
				int index_two = 999999;
				string atom_one;
				string atom_two;
				atom_one = i;
				atom_one += to_string(time_one);
				atom_two = i;
				atom_two += to_string(time_two);
				for(int j = 0; j < all_atoms.size(); j++)
				{
					if(all_atoms[j] == atom_one)
					{
						index_one = j + 1;
					}
					if(all_atoms[j] == atom_two)
					{
						index_two = j + 1;
					}
				}
				// front_end_output_file << "-" << i << time_one << " -" << i << time_two << endl;
				front_end_output_file << "-" << index_one << " -" << index_two << endl;
				time_two++;
			}
			time_one++;
		}
	}
	
	front_end_output_file.close();
	
	return 0;
}