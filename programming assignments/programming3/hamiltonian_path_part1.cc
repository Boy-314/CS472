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
	
	// open input file and store number of vertices and all the edges
	ifstream front_end_file("FrontEndInput.txt");
	front_end_file >> num_of_vertices;
	while(front_end_file >> vertex_start >> vertex_end)
	{
		edge temp = {vertex_start, vertex_end};
		graph.push_back(temp);
	}
	front_end_file.close();
	
	// get a set of vertices
	set<string> vertices;
	for(int i = 0; i < graph.size(); i++)
	{
		vertices.insert(graph[i].vertex_start);
		vertices.insert(graph[i].vertex_end);
	}
	
	// rule: every vertex is traversed at some time
	for(auto i : vertices)
	{
		for(int j = 0; j < num_of_vertices; j++)
		{
			cout << i << j + 1 << " ";
		}
		cout << endl;
	}
	
	// rule: no pair of vertices are traversed at the same time
	int time = 1;
	for(auto i : vertices)
	{
		for(auto j : vertices)
		{
			if(i == j)
			{
				continue;
			}
			cout << "-" << i << time << " -" << j << time << endl;
		}
		time++;
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
			cout << "-" << missing_edges[i].vertex_start << time << " -" << missing_edges[i].vertex_end << time + 1 << endl;
			time++;
		}
	}
	
	// optional rule: at every time there is a vertex
	
	// optinoal rule: no vertex is traversed more than once
	
	return 0;
}