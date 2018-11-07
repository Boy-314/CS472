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
		V[L - 1] = 1;
	}
	else if(L < 0)
	{
		V[-L - 1] = 0;
	}
	return V;
}

// method to check if there are any pure literals
int pure_literals(vector<int> ATOMS, vector<clause> S, vector<int> V)
{
	bool p_literal = true;
	// iterate through each atom in S
	for(auto line : S)
	{
		for(auto atom : line.atoms)
		{
			// check if its negation appears
			for(auto check_line : S)
			{
				p_literal = true;
				for(auto check_atom : line.atoms)
				{
					if(check_atom == -atom)
					{
						p_literal = false;
						break;
					}
				}
				if(!p_literal)
				{
					break;
				}
			}
			if(p_literal)
			{
				return atom;
			}
		}
	}
	return 0;
}

// method to check if there are any single literals
bool single_literals(vector<int> ATOMS, vector<clause> S, vector<int> V)
{
	bool exists = true;
	for(auto line : S)
	{
		if((line.atoms).size() == 1)
		{
			exists = true;
		}
	}
	if(exists)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// vector V: -1 if UNBOUND, 0 if FALSE, 1 if TRUE
vector<int> dp1(vector<int> ATOMS, vector<clause> S, vector<int> V)
{
	vector<int> NIL;
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
			for(int A = 0; A < ATOMS.size(); A++)
			{
				// cout << "A: " << A << endl;
				if(V[A] == -1)
				{
					V[A] = 1;
				}
			}
			return V;
		}
		// failure: some clause is unsatisfiable under V
		else if(is_empty)
		{
			return NIL;
		}
		
		// easy cases: pure literal elimination and single literal forced assignment
		
		// pure literal elimination such that the negation of L does not appear in S
		else if(pure_literals(ATOMS, S, V) != 0)
		{
			int L = pure_literals(ATOMS, S, V);
			// cout << "pure literal: " << L << "\n";	
			V = obviousAssign(L,V);
			// if(L < 0)
			// {
				// cout << "V[" << -L << "]: " << V[-L] << endl;
			// }
			// if(L > 0)
			// {
				// cout << "V[" << L << "]: " << V[L] << endl;
			// }
			
			// delete every clause containing L from S
			// vector<int> to_remove;
			int line_counter = 0;
			for(auto line : S)
			{
				vector<int>::iterator iter;
				for(iter = (line.atoms).begin(); iter != (line.atoms).end(); )
				{
					if(*iter == L || *iter == -L)
					{
						S.erase(S.begin() + line_counter);
						break;
					}
					else
					{
						++iter;
					}
				}
				line_counter++;
			}
			// cout << S.size() << endl;
		}
		
		// single literal forced assignment
		else if(single_literals(ATOMS, S, V))
		{
			// find the single literal
			int L;
			for(auto line : S)
			{
				if((line.atoms).size() == 1)
				{
					L = (line.atoms)[0];
				}
			}
			V = obviousAssign(L,V);
			S = propagate(L,S,V);
		}
		
		// no easy cases found, exit the loop
		else
		{
			break;
		}
	}
	
	// hard case: pick some atom and try each assignment in turn
	int A;
	for(auto atom : ATOMS)
	{
		if(V[atom] == -1)
		{
			A = atom;
			break;
		}
	}
	// set A to be true
	V[A] = 1;
	vector<clause> S1 = S;
	S1 = propagate(A,S1,V);
	vector<int> VNEW = dp1(ATOMS,S1,V);
	if(VNEW != NIL)
	{
		return VNEW;
	}
	
	V[A] = 0;
	S1 = propagate(A,S,V);
	return dp1(ATOMS,S1,V);
}

int main()
{
	// parse set of clauses from the output of the front end
	vector<clause> set_of_clauses;
	vector<int> ATOMS;
	ifstream front_end_output_file;
	front_end_output_file.open("FrontEndOutput.txt");
	string line;
	while(getline(front_end_output_file, line))
	{
		istringstream iss(line);
		int atom;
		vector<int> temp;
		bool zero = false;
		while(iss >> atom)
		{
			temp.push_back(atom);
		}
		clause atoms = {temp};
		set_of_clauses.push_back(atoms);
	}
	front_end_output_file.close();
	int counter = 0;
	while(set_of_clauses[counter].atoms[0] > 0)
	{
		for(auto i : set_of_clauses[counter].atoms)
		{
			ATOMS.push_back(i);
		}
		counter++;
	}
	vector<int> V;
	for(auto i : ATOMS)
	{
		V.push_back(-1);
	}
	set_of_clauses.erase(set_of_clauses.begin() + set_of_clauses.size());
	// for(auto i : set_of_clauses)
	// {
		// for(auto j : i.atoms)
		// {
			// cout << j;
		// }
		// cout << endl;
	// }
	vector<int> output = dp1(ATOMS, set_of_clauses, V);
	ofstream davis_putnam_output_file;
	davis_putnam_output_file.open("DavisPutnamOutput.txt");
	counter = 0;
	// cout << output.size() << endl;
	for(auto i : output)
	{
		// cout << i << endl;
		counter++;
		if(i == 0)
		{
			davis_putnam_output_file << counter << " F\n";
		}
		if(i == 1)
		{
			davis_putnam_output_file << counter << " T\n";
		}
	}
	davis_putnam_output_file << 0;
	davis_putnam_output_file.close();
	return 0;
}