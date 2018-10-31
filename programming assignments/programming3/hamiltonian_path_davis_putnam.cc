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

// data structure for each individual clause
struct clause
{
	vector<int> atoms;
};

/*
dp(in  ATOMS : set of propositional atoms; S : Set of propositional formulas in CNF)
{
	V : array[ATOMS];

	for (A in ATOMS) 
	{
		do V[A] = UNBOUND;
		return dp1(ATOMS,S,V)
	}	
}
*/

/*
dp1(ATOMS,S,V)
{
	loop
	{
		// base case:
		if(S is empty) // all clauses are satisfied
		{
			for(A in ATOMS)
			{
				if(V[a] == UNBOUND)
				{
					assign V[A] either TRUE or FALSE;
				}
				return V;
			}
		}
		else if(some clause in S is empty) // failure: some clause is unsatisfiable under V
		{
			return NIL;
		}
		
		// easy cases: pure literal elimination and forced assignment
		else if(there exists a literal L in S such that the negation of L does not appear in S) // pure literal elimination
		{
			V = obviousAssign(L,V);
			delete every clause containing L from S;
		}
		else if(there exists a clause C in S containing a single literal L) // forced assignment
		{
			V = obviousAssign(L,V);
			S = propagate(atom(L), S, V);
		}
		else
		{
			break;
		}
	}
	
	// hard case: pick some atom and try each assignment in turn
	pick atom A such that V[A] == UNBOUND; // try one assignment
	V[A] = TRUE;
	S1 = copy(S);
	S1 = propagate(A,S1,V);
	VNEW = dp1(ATOMS,S1,V);
	if(VNEW != NULL)
	{
		return VNEW; // found a satisfying valuation
	}
	
	// if V[A] = TRUE didn't work, try V[A] = FALSE;
	V[A] = FALSE;
	S1 = propagate(A,S,V);
	return(dp1(ATOMS,S1,V)); // either found a satisfying valuation or backtrack
}
*/

/*
// propagate the effect of assigning atom A to be value V.
// delete every clause where A appears with sign V.
// delete every literal where A appears with sign not V.
propagate(A,S,V)
{
	for(each clause C in S)
	{
		if((A in C and V[A] == FALSE) or (~A in C and V[A] == FALSE))
		{
			delete C from S
		}
		else if(A in C and V[A] == FALSE)
		{
			delete A from C
		}
		else if(~A in C and V[A] == TRUE)
		{
			delete ~A from C
		}
	}
	return S;
}
*/

/*
// given a literal L with atom A, make V[A] the sign indicated by L.
obviousAssign(L,V)
{
	if(L is an atom A)
	{
		V[A] = TRUE;
	}
	else if(L has the form ~A)
	{
		V[A] = FALSE;
	}
}
*/

int main()
{
	// parse set of clauses from the output of the front end
	vector<clause> set_of_clauses;
	ifstream front_end_output_file;
	front_end_output_file.open("FrontEndOutput.txt");
	string line;
	while(getline(front_end_output_file, line))
	{
		istringstream iss(line);
		int atom;
		vector<int> temp;
		while(iss >> atom)
		{
			temp.push_back(atom);
		}
		clause atoms = {temp};
		set_of_clauses.push_back(atoms);
	}
	for(int i = 0; i < set_of_clauses.size(); i++)
	{
		for(int j = 0; j < set_of_clauses[i].atoms.size(); j++)
		{
			cout << set_of_clauses[i].atoms[j] << " ";
		}
		cout << endl;
	}
	return 0;
}