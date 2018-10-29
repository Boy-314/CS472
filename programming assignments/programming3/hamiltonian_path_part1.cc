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
	/*
	for all the edges that are not in the graph
		for all the vertices
			not edgebegin or not edgeend
	*/
	
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
	vector<edge> missing_edges;
	for(int i = 0; i < complete_graph.size(); i++)
	{
		for(int j = 0; j < edges.size(); j++)
		{
			if(edges[j].vertex_start != complete_graph[i].vertex_start && edges[j].vertex_end != complete_graph[i].vertex_end)
		}
	}
	
	return 0;
}