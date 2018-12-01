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
	int start_phrase;
	int end_phrase;
	
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
	string left_child;
	string right_child;
	double probability;
};

// struct for indiviual entries in the lexicon
struct lexicon_entry
{
	string part_of_speech;
	string word;
	double probability;
};

// methods for comparing grammar and lexicon entries
bool compare_gentry(const grammar_entry& A, const grammar_entry& B)
{
	if(A.root == B.root && A.left_child == B.left_child && A.right_child == B.right_child)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool compare_lentry(const lexicon_entry& A, const lexicon_entry& B)
{
	if(A.part_of_speech == B.part_of_speech && A.word == B.word)
	{
		return true;
	}
	else
	{
		return false;
	}
}

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


// method to convert string to grammar entry
pair<grammar_entry,int> prefix_to_grammar_entry(vector<string> S, int K)
{
	// cout << "K: " << K << endl;
	grammar_entry N;
	N.root = S[K];
	K++;
	
	if(S[K-1][0] == '*')
	{
		cout << "S[K-1]: " << S[K-1] << endl;
		for(int i = 0; i < 2; i++)
		{
			pair<grammar_entry,int> temp = prefix_to_grammar_entry(S,K);
			if(i == 0)
			{
				N.left_child = temp.first.root;
			}
			if(i == 1)
			{
				N.right_child = temp.first.root;
			}
		}
	}
	pair<grammar_entry,int> ret;
	ret.first = N;
	ret.second = K;
	return ret;
}

// method to convert string to lexicon entry
pair<lexicon_entry,int> prefix_to_lexicon_entry(vector<string> S, int K)
{
	lexicon_entry N;
	N.part_of_speech = S[K];
	K++;
	
	if(S[K-1][0] == '+')
	{
		for(int i = 0; i < 1; i++)
		{
			pair<lexicon_entry,int> temp = prefix_to_lexicon_entry(S,K);
			N.word = temp.first.part_of_speech;
		}
	}
	pair<lexicon_entry,int> ret;
	ret.first = N;
	ret.second = K;
	return ret;
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
		
		// convert training set to grammar and lexicon vectors
		vector<grammar_entry> grammar;
		vector<lexicon_entry> lexicon;
		for(int i = 0; i < lines; i++)
		{
			vector<string> temp = split(training_input[i]);
			for(int j = 0; j < temp.size(); j++)
			{
				// if its a phrase marker
				if(temp[j][0] == '*')
				{
					grammar_entry g;
					g.root = temp[j];
					g.left_child = temp[j+1];
					char first = temp[j+1][0];
					char second;
					if(first == '+')
					{	
						second = temp[j+3][0];
						g.right_child = temp[j+3];
					}
					if(first == '*')
					{
						second = temp[j+6][0];
						g.right_child = temp[j+6];
					}
					grammar.push_back(g);
				}
				
				// if its a part of speech
				if(temp[j][0] == '+')
				{
					lexicon_entry l;
					l.part_of_speech = temp[j];
					l.word = temp[j+1];
					lexicon.push_back(l);
				}
			}
		}
		
		// calculate probabilities for grammar vector
		for(int i = 0; i < grammar.size(); i++)
		{
			grammar_entry g = grammar[i];
			
			// match_counter: keeps track of number of times the entire grammar entry is equal
			// root_counter: keeps track of number of times only the phrase marker matches
			double match_counter = 0;
			double root_counter = 0;
			for(int j = 0; j < grammar.size(); j++)
			{
				// if root, left_child, and right_child match up, increment the match counter
				if(g.root == grammar[j].root && g.left_child == grammar[j].left_child && g.right_child == grammar[j].right_child)
				{
					match_counter++;
				}
				
				// if only the root matches, increment the root counter
				if(g.root == grammar[j].root)
				{
					root_counter++;
				}
			}
			
			// update probabilities based on counter variables
			grammar[i].probability = match_counter/root_counter;
		}
		
		// calculate probabilities for lexicon vector
		for(int i = 0; i < lexicon.size(); i++)
		{
			lexicon_entry l = lexicon[i];
			
			// match_counter: keeps track of number of times the entire lexicon entry is equal
			// pos_counter: keeps track of number of times only the part of speech matches
			double match_counter = 0;
			double pos_counter = 0;
			for(int j = 0; j < lexicon.size(); j++)
			{
				// if part of speech and word all match up, increment the match counter
				if(l.part_of_speech == lexicon[j].part_of_speech && l.word == lexicon[j].word)
				{
					match_counter++;
				}
				
				// if only the part of speech matches, increment the pos counter
				if(l.part_of_speech == lexicon[j].part_of_speech)
				{
					pos_counter++;
				}
			}
			
			// update probabilities based on counter variables
			lexicon[i].probability = match_counter/pos_counter;
		}
		
		// output a listing of the grammatical rules (including lexical rules), with associated probability
		cout << "Grammar\n";
		vector<grammar_entry> used;
		bool dup = false;
		for(auto i : grammar)
		{
			for(auto j : used)
			{
				// cout << j.root << ", " << j.left_child << ", " << j.right_child << ", " << j.probability << endl;
				if(i.root == j.root && i.left_child == j.left_child && i.right_child == j.right_child && i.probability == j.probability)
				{
					dup = true;
					break;
				}
			}
			if(!dup)
			{
				cout << i.root.substr(1,i.root.length()) << " -> " << i.left_child.substr(1,i.left_child.length()) << " " << i.right_child.substr(1,i.right_child.length()) << " [" << i.probability << "]\n";
			}
			if(!dup)
			{
				used.push_back(i);
			}
			dup = false;
		}
		cout << endl;
		cout << "Lexicon\n";
		
		// run cyk parse
	}
	return 0;
}