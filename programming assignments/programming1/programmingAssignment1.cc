#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <iterator>
#include <bits/stdc++.h>
#include <stdio.h>
#include <ctype.h>
using namespace std;

int TOTALGRAMMARRULES = 18;
int LEXICONSIZE = 12;
int N;

// setup nonterms to index P chart
vector<string> NonTerms = {"S","NP","PP","PPList","VerbAndObject","VPWithPPList","Noun","Prep","Verb"};


// tree class
class Tree
{
	public:
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
struct grammarEntry
{
	string root;
	string leftChild;
	string rightChild;
	double probability;
};

// struct for indiviual entries in the lexicon
struct lexiconEntry
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

// main cyk-parse algorithm
vector<vector<vector<Tree>>> CYKPARSE(vector<grammarEntry> grammar,vector<lexiconEntry> lexicon)
{	
	// get user input
	string userInput;
	cout << "Enter your sentence:" << endl;
	getline(cin,userInput);
	
	// convert user input into all lower case
	transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
	
	// split user input
	vector<string> sentence = split(userInput);
	
	N = sentence.size();
	
	// setup P chart
	Tree placeholder = {-1,-1,-1,"XXXXXXX",NULL,NULL,0};
	vector<vector<vector<Tree>>> P(LEXICONSIZE,vector<vector<Tree>>(N,vector<Tree>(N,placeholder)));
		
	for(int i = 0; i < N; i++)
	{
		string word = sentence[i];
		
		// for each rule of form "POS --> Word [prob]" in the grammar
		for(int j = 0; j < lexicon.size(); j++)
		{
			// find index of POS
			int POS = 0;
			while(NonTerms[POS] != lexicon[j].partOfSpeech)
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
			for(int NonTerm = 0; NonTerm < NonTerms.size(); NonTerm++)
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
						while(NonTerms[M] != grammar[grammarRule].root)
						{
							M++;
						}
						int Y = 0;
						while(NonTerms[Y] != grammar[grammarRule].leftChild)
						{
							Y++;
						}
						int Z = 0;
						while(NonTerms[Z] != grammar[grammarRule].rightChild)
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

// print tree method
void printTree(Tree tree, int INDENT)
{
	// if not a grammatically correct sentence
	if(tree.prob == 0)
	{
		cout << "This sentence cannot be parsed" << endl;
		return;
	}
	
	// otherwise output tree with pre-order search
	for(int spaces = 0; spaces < INDENT; spaces++)
	{
		cout << ' ';
	}
	cout << NonTerms[tree.phrase];
	if(tree.word != "XXXXXXX")
	{
		cout << " " << tree.word << endl;
	}
	else
	{
		cout << endl;
		printTree(*(tree.left),INDENT + 3);
		printTree(*(tree.right),INDENT + 3);
	}
}

int main()
{	
	// setup grammar
	vector<grammarEntry> grammar = {{"S","Noun","Verb",0.2},{"S","Noun","VerbAndObject",0.3},{"S","Noun","VPWithPPList",0.1},{"S","NP","Verb",0.2},{"S","NP","VerbAndObject",0.1},{"S","NP","VPWithPPList",0.1},{"NP","Noun","PP",0.8},{"NP","Noun","PPList",0.2},{"PP","Prep","Noun",0.6},{"PP","Prep","NP",0.4},{"PPList","PP","PP",0.6},{"PPList","PP","PPList",0.4},{"VerbAndObject","Verb","Noun",0.5},{"VerbAndObject","Verb","NP",0.5},{"VPWithPPList","Verb","PP",0.3},{"VPWithPPList","Verb","PPList",0.1},{"VPWithPPList","VerbAndObject","PP",0.4},{"VPWithPPList","VerbAndObject","PPList",0.2}};
	
	// setup lexicon
	vector<lexiconEntry> lexicon = {{"Noun","amy",0.1},{"Noun","dinner",0.2},{"Noun","fish",0.2},{"Noun","streams",0.1},{"Noun","swim",0.2},{"Noun","tuesday",0.2},{"Prep","for",0.5},{"Prep","in",0.3},{"Prep","on",0.2},{"Verb","ate",0.7},{"Verb","streams",0.1},{"Verb","swim",0.2}};
	
	// run the cyk parser algorithm
	vector<vector<vector<Tree>>> P = CYKPARSE(grammar,lexicon);
	
	// print the tree
	printTree(P[0][0][N - 1],0);
	
	// print out probability
	cout << "Probability = " << P[0][0][N - 1].prob;
	return 0;
}