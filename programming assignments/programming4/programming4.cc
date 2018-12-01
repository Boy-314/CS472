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

// tree struct
struct Tree
{
	// the non-terminal
	int phrase;
	
	// indices of starting and ending word
	int startPhrase;
	int endPhrase;
	
	// if a leaf, then the word
	string word;
	
	Tree * left;
	Tree * right;
	double prob;
};

// struct for individual entries in the grammar
struct grammar_entry
{
	string root;
	string leftChild;
	string rightChild;
	double probability;
};

// struct for indiviual entries in the lexicon
struct lexicon_entry
{
	string partOfSpeech;
	string word;
	double probability;
};

// method for splitting strings
vector<string> split(string s)
{
	int len = s.length();
	vector<string> output;
	for(int i = 0, j = 0; i < len; i++)
	{
		if(s[i] == ' ')
		{
			string word = s.substr(j,i - j);
			j = i + 1;
			output.push_back(word);
		}
		if(i == len - 1)
		{
			string word = s.substr(j,i - j + 1);
			output.push_back(word);
		}
	}
	return output;
}

// method to find end of a grammar entry
int find_end(vector<string> training_set)
{
}

int main(int argc, char * argv[])
{
	// deal with incorrect command line arguments
	if(argc != 3)
	{
		cout << "invalid command line arguments\n";
		return 0;
	}
	
	// if command line arguments are OK
	else if(argc == 3)
	{
		char* end;
		int lines = strtol(argv[1],&end,10);
		string flag = argv[2];
		
		// convert user input to data that the program can handle
		// final data is training_set and test_set
		string line;
		vector<string> training_input;
		vector<string> test_input;
		for(int i = 0; i < lines; i++)
		{
			getline(cin,line);
			training_input.push_back(line);
		}
		for(int i = 0; i < lines; i++)
		{
			getline(cin,line);
			test_input.push_back(line);
		}
		
		vector<string> training_set;
		vector<string> test_set;
		vector<string> training_words;
		vector<string> test_words;
		for(int i = 0; i < lines; i++)
		{
			vector<string> training_temp = split(training_input[i]);
			for(auto j : training_temp)
			{
				training_set.push_back(j);
			}
			vector<string> test_temp = split(test_input[i]);
			for(auto j : test_temp)
			{
				test_set.push_back(j);
			}
		}
		
		// TODO: convert training set to grammar and lexicon vectors
		vector<grammar_entry> grammar;
		vector<lexicon_entry> lexicon;
	}
	return 0;
}