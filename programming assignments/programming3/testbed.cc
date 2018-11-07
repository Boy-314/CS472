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

int main()
{
	vector<int> original = {1,2,3,4};
	vector<int> copy = original;
	original[0]++;
	for(auto i : original)
	{
		cout << i << endl;
	}
	cout << endl;
	for(auto i : copy)
	{
		cout << i << endl;
	}
	return 0;
}