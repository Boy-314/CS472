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

// methods for comparing clause objects
bool operator!=(const clause& lhs, const clause& rhs)
{
	if(lhs.atoms.size() != rhs.atoms.size())
	{
		return true;
	}
	else if(lhs.atoms.size() == rhs.atoms.size())
	{
		for(int i = 0; i < lhs.atoms.size(); i++)
		{
			if(lhs.atoms[i] != rhs.atoms[i])
			{
				return true;
			}
		}
	}
	else
	{
		return false;
	}
}

bool operator==(const clause& lhs, const clause& rhs)
{
	if(lhs.atoms.size() != rhs.atoms.size())
	{
		return false;
	}
	else if(lhs.atoms.size() == rhs.atoms.size())
	{
		for(int i = 0; i < lhs.atoms.size(); i++)
		{
			if(lhs.atoms[i] != rhs.atoms[i])
			{
				return false;
			}
		}
	}
	else
	{
		return true;
	}
}

/*
propagate the effect of assigning atom A to be value V.
delete every clause where A appears with sign V.
delete every literal where A appears with sign not V.
*/
vector<clause> propagate(int A, vector<clause> S, vector<int> V)
{
	// for each clause C in S
	for(auto C : S)
	{
		// if A in C and V[A] == TRUE or ~A in C and V[A] == FALSE
		if(((find((C.atoms).begin(), (C.atoms).end(), A) != (C.atoms).end()) && (V[A] == 1)) && (((find((C.atoms).begin(), (C.atoms).end(), -A) != (C.atoms).end()) && (V[A] == 0))))
		{
			// delete C from S
			int index = 0;
			while(S[index] != C)
			{
				index++;
			}
			S.erase(S.begin() + index);
		}
		// else if A in C and V[A] == FALSE
		else if((find((C.atoms).begin(), (C.atoms).end(), A) != (C.atoms).end()) && (V[A] == 0))
		{
			// delete A from C in S
			int index = 0;
			while((C.atoms)[index] != A)
			{
				index++;
			}
			int counter = 0;
			while(S[counter] != C)
			{
				counter++;
			}
			(S[counter].atoms).erase((S[counter].atoms).begin() + index);
		}
		// else if ~A in C and V[A] == TRUE
		else if((find((C.atoms).begin(), (C.atoms).end(), -A) != (C.atoms).end()) && (V[A] == 1))
		{
			// delete ~A from C in S
			int index = 0;
			while((C.atoms)[index] != -A)
			{
				index++;
			}
			int counter = 0;
			while(S[counter] != C)
			{
				counter++;
			}
			(S[counter].atoms).erase((S[counter].atoms).begin() + index);
		}
	}
	return S;
}

/*
given a literal L with atom A, make V[A] the sign indicated by L.
*/
vector<int> obviousAssign(int L, vector<int> V)
{
	if(L > 0)
	{
		V[L] = 1;
	}
	else if(L < 0)
	{
		V[L] = 0;
	}
	return V;
}

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

// method to check if there are any pure literals
int pure_literals(vector<int> ATOMS, vector<clause> S, vector<int> V)
{
	int literal = 0;
	bool exists = true;
	for(auto atom : ATOMS)
	{
		bool pos_flag = false;
		bool neg_flag = false;
		for(auto C : S)
		{
			for(auto a : C.atoms)
			{
				if(!pos_flag && !neg_flag)
				{
					if(a == atom)
					{
						pos_flag = true;
					}
					if(-a == atom)
					{
						neg_flag = true;
					}
				}
				else if(pos_flag)
				{
					if(-a == atom)
					{
						literal = atom;
						exists = false;
					}
				}
				else if(neg_flag)
				{
					if(a == atom)
					{
						literal = atom;
						exists = false;
					}
				}
			}
		}
	}
	return literal;
}

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
	if(VNEW != NIL)
	{
		return VNEW; // found a satisfying valuation
	}
	
	// if V[A] = TRUE didn't work, try V[A] = FALSE;
	V[A] = FALSE;
	S1 = propagate(A,S,V);
	return(dp1(ATOMS,S1,V)); // either found a satisfying valuation or backtrack
}
*/
vector<int> dp1(vector<int> ATOMS, vector<clause> S, vector<int> V)
{
	// loop as long as there are easy cases to cherry pick
	while(true)
	{
		// check if any clause in S is empty
		bool is_empty = false;
		for(auto C : S)
		{
			if((C.atoms).empty())
			{
				is_empty = true;
			}
		}
		
		// base case
		// success: all clauses are satisfied
		if(S.empty())
		{
			for(auto A : ATOMS)
			{
				if(V[A] == -1)
				{
					V[A] = 1;
				}
				return V;
			}
		}
		// failure: some clause is unsatisfiable under V
		else if(is_empty)
		{
			vector<int> NIL;
			return NIL;
		}
		
		// easy cases: pure literal elimination and forced assignment
		// pure literal elimination such that the negation of L does not appear in S
		else if(pure_literals(ATOMS, S, V) != 0)
		{
			int L = pure_literals(ATOMS, S, V);
			V = obviousAssign(L,V);
			// delete every clause containing L from S
			
		}
	}
}

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
	return 0;
}