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

int N;

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

// main cyk-parse algorithm
vector<vector<vector<Tree>>> CYKPARSE(vector<grammar_entry> grammar,vector<lexicon_entry> lexicon, string user_input, vector<string> non_terms)
{	
	// UNUSED: convert user input into all lower case
	// transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
	
	// split user input
	vector<string> sentence = split(user_input);
	
	N = sentence.size();
	
	// setup P chart
	Tree placeholder = {-1,-1,-1,"XXXXXXX",NULL,NULL,0};
	vector<vector<vector<Tree>>> P(lexicon.size(),vector<vector<Tree>>(N,vector<Tree>(N,placeholder)));
		
	for(int i = 0; i < N; i++)
	{
		string word = sentence[i];
		
		// for each rule of form "POS --> Word [prob]" in the grammar
		for(int j = 0; j < lexicon.size(); j++)
		{
			// find index of POS
			int POS = 0;
			while(non_terms[POS] != lexicon[j].part_of_speech)
			{
				POS++;
			}
			if(word == lexicon[j].word)
			{
				Tree tree = {POS,i,i,word,NULL,NULL,lexicon[j].probability};
				P[POS][i][i] = tree;
			}
		}
	}
	
	// length = length of the phrase
	for(int length = 2; length < N + 1; length++)
	{
		// i = start of the phrase
		for(int i = 0; i < N + 1 - length; i++)
		{
			// j = end of the phrase
			int j = i + length - 1;
			for(int NonTerm = 0; NonTerm < non_terms.size(); NonTerm++)
			{
				Tree tree = {NonTerm,i,j,"XXXXXXX",NULL,NULL,0.0};
				P[NonTerm][i][j] = tree;
				
				// k = end of first subphrase
				for(int k = i; k < j; k++)
				{
					// for each rule of form "M -> Y,Z [prob]" in the grammar
					for(int grammarRule = 0; grammarRule < grammar.size(); grammarRule++)
					{
						// find indices of M,Y,Z
						int M = 0;
						while(non_terms[M] != grammar[grammarRule].root)
						{
							M++;
						}
						int Y = 0;
						while(non_terms[Y] != grammar[grammarRule].left_child)
						{
							Y++;
						}
						int Z = 0;
						while(non_terms[Z] != grammar[grammarRule].right_child)
						{
							Z++;
						}
						double newProb = P[Y][i][k].prob * P[Z][k + 1][j].prob * grammar[grammarRule].probability;
						if(newProb > P[M][i][j].prob)
						{
							P[M][i][j].left = &(P[Y][i][k]);
							P[M][i][j].right = &(P[Z][k + 1][j]);
							P[M][i][j].prob = newProb;
						}
					}
				}
			}
		}
	}
	
	return P;
}

// stringstream that will hold each sentence as a whole (from the printTree pre-order traversal)
stringstream buffer;

// print tree method
void printTree(Tree tree, vector<string> non_terms, string flag)
{
	// if not a grammatically correct sentence
	if(tree.prob == 0)
	{
		buffer << "This sentence cannot be parsed. Wrong";
		if(flag == "yes")
		{
			cout << "This sentence cannot be parsed. Wrong" << endl;
		}
		return;
	}
	
	// otherwise output tree with pre-order search
	buffer << non_terms[tree.phrase] << " ";
	if(flag == "yes")
	{
		cout << non_terms[tree.phrase] << " ";
	}
	
	if(tree.word != "XXXXXXX")
	{
		buffer << tree.word << " ";
		if(flag == "yes")
		{
			cout << tree.word << " ";
		}
	}
	else
	{
		printTree(*(tree.left),non_terms,flag);
		printTree(*(tree.right),non_terms,flag);
	}
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
		cout << "\nGrammar\n";
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
				used.push_back(i);
			}
			dup = false;
		}
		cout << endl;
		// reset temporary variables
		vector<lexicon_entry> temp;
		dup = false;
		cout << "Lexicon\n";
		for(auto i : lexicon)
		{
			for(auto j : temp)
			{
				if(i.part_of_speech == j.part_of_speech && i.word == j.word)
				{
					dup = true;
					break;
				}
			}
			if(!dup)
			{
				cout << i.part_of_speech.substr(1,i.part_of_speech.length()) << " -> " << i.word << " [" << i.probability << "]\n";
				temp.push_back(i);
			}
			dup = false;
		}
		
		// create non_terms vector
		vector<string> non_terms;
		vector<string> exists;
		dup = false;
		for(auto i : grammar)
		{
			for(auto j : exists)
			{
				if(i.root == j)
				{
					dup = true;
					break;
				}
			}	
			if(!dup)
			{
				non_terms.push_back(i.root);
				exists.push_back(i.root);
			}
			dup = false;
		}
		dup = false;
		exists.clear();
		for(auto i : lexicon)
		{
			for(auto j : exists)
			{
				if(i.part_of_speech == j)
				{
					dup = true;
					break;
				}
			}
			if(!dup)
			{
				non_terms.push_back(i.part_of_speech);
				exists.push_back(i.part_of_speech);
			}
			dup = false;
		}
		cout << endl;
		
		// create user input from test_input
		vector<string> sentences;
		for(auto j : test_input)
		{
			vector<string> temp = split(j);
			string sentence = "";
			for(auto k : temp)
			{
				if(k[0] != '*' && k[0] != '+')
				{
					sentence += k + " ";
				}
			}
			sentence = sentence.substr(0,sentence.length() - 1);
			sentences.push_back(sentence);
		}
		// for(auto j : sentences)
		// {
			// cout << j << endl;
		// }
		
		// run cyk parse
		double Y = 0.0;
		if(flag == "yes")
		{
			cout << "Parses\n";
		}
		for(int i = 0; i < sentences.size(); i++)
		{
			vector<vector<vector<Tree>>> P = CYKPARSE(grammar,lexicon,sentences[i],non_terms);
			
			// print the tree if flag is yes
			printTree(P[0][0][N - 1],non_terms,flag);
			// cout << endl << buffer.str() << endl << test_input[i] << endl;
			string output = buffer.str().substr(0,buffer.str().length() - 1);
			// cout << output.length() << ", " << test_input[i].length() << endl;
			if(output.compare(test_input[i]) == 0)
			{
				if(flag == "yes")
				{
					cout << "Right\n";
				}
				Y++;
			}
			else if(buffer.str() == "This sentence cannot be parsed. Wrong")
			{
			}
			else
			{
				if(flag == "yes")
				{
					cout << "Wrong\n";
				}
			}
			buffer.str("");
		}
		
		// output probabilities
		double X = lines;
		cout << "\nThe parser was tested on " << X << " sentences. It got " << Y << " correct, for an accuracy of " << Y/X << ".\n";
	}
	
	return 0;
}