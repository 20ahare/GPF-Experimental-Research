#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdlib.h>

using namespace std;

// Global Variables 
int m;
int a;

//Function Definitions
long long op(long long in);
long long MaxPrime(long long num);

// Directed Graph Class to organize data
class Graph
{
private:

	// Define a node structure to populate graph
	struct Node
	{
		long long key = -1;
		Node* next = nullptr;
		bool isLast = true;
		int depth = -1;
		int land = -1;
	};
	// Initialize variables and structures

	// Unnecessary vector, stores key values in same order as vals, can be taken out of functions and rewritten as time allows
	vector<long long> vals;

	// Stores node pointers of all nodes in graph
	vector<Node*> pts;
	vector<vector<int>> entrances;
	// List of key values that are in limit cycles
	vector<int> cycles = {};
	// largest depth value found 
	int maxDepth = 0;

public:

	// Takes in a vector that provides a list of weights corresponding to the existing nodes  
	Node* addNode(long long val)
	{
		// if the key value exists already return that node
		for (long long i = 0; i < vals.size(); i++)
		{
			if (val == vals[i])
			{
				return pts[i];
			}

		}
		// otherwise generate new node
		Node* newnode = new Node;

		// Add new node to key value list and node list
		vals.push_back(val);
		pts.push_back(newnode);

		// Define the key and next properties of the node
		newnode->key = val;
		newnode->next = addNode(op(val));
		// If the next node has not defined a depth this is the start of the cycle, replace its depth with 0 to denote for future
		if (newnode->next->depth < 0)
			newnode->next->depth = 0;
		else
			newnode->depth = 1 + newnode->next->depth;

		return newnode;
	}

	// Displays output results of graph operations
	void disp()
	{
	
		cout << "Maximum depth: " << maxDepth << endl;
		for (int i = 0; i < cycles.size(); i++)
		{
			cout << entrances[i][0] << " : ";

			for (int j = 1; j <= maxDepth; j++)
				cout << entrances[i][j] << " | " ;
			cout << endl;

			for (int k = 0; k < 6 * maxDepth; k++)
				cout << "-";
			cout << endl;
		}
		
	}

	void print(int n)
	{
		string dir = "\StoredGraphs\\";
		ofstream output(dir +"m = " + to_string(m) + ", a = " + to_string(a) + ", n = " + to_string(n) + ".txt");
		for (long long i = 0; i < pts.size(); i++)
		{
			output << pts[i]->key << ">" << pts[i]->next->key << endl;
		}
			
		output << "Depth and Land Values for each seed: " << endl;
		for (long long j = 0; j < pts.size(); j++)
		{
			output << pts[j]->key << " | " << pts[j]->depth << " | " << pts[j]->land << endl;
		} 


		output << "Maximum depth: " << maxDepth << endl;

		for (int i = 0; i < cycles.size(); i++)
		{
			output << entrances[i][0] << " : ";

			for (int j = 1; j <= maxDepth; j++)
				output << entrances[i][j] << " | ";
			output << endl;

			for (int k = 0; k < 6 * maxDepth+1; k++)
				output << "-";
			output << endl;
		}

		output.close();
	}

	// Sets landing values for all nodes
	void setLand()
	{
		for (long long i = 0; i < pts.size(); i++)
		{
			Node* index = pts[i];
			while (index->depth != 0)
			{
				index = index->next;
			}
			pts[i]->land = index->key;
		}
	}
	// Finalizes Depth and Land values accross all nodes in graph
	void calcData()
	{
		for (long long i = 0; i < pts.size(); i++)
		{
			Node* cur = pts[i];
			if (find(cycles.begin(), cycles.end(), cur->land) == cycles.end())
				cycles.push_back(cur->land);

			if (cur->depth > maxDepth)
				maxDepth = cur->depth;
		}

		
		// Declare a 2d vector with sizing pre-determined by the iteration before searching for each 
		entrances.resize(cycles.size(), vector<int>(maxDepth + 1, 0));

		// Initialize
		for (long long n = 0; n < pts.size(); n++)
		{
			
			// Define iterator that finds the index of the land value in cycles vector
			vector<int>::iterator I = find(cycles.begin(), cycles.end(), pts[n]->land);

			// define i and j to be the indexed land value and the depth value of the current node
			int i = distance(cycles.begin(), I);
			int j = pts[n]->depth;

			if (j == 0)
				entrances[i][j] = pts[n]->land;
			else
				entrances[i][j]++;
		}
	}

};


// Main function that executes at run-time
int main(int argc, char* argv[])
{
	// Initialize graph and max seed value
	Graph g;
	long long n;

	// Request Iterator Values m, a from user
	cout << "Iterator is in the form GPF(p * m + a) = p'" << endl<< endl << "Please enter values for m and a respectively:";
	cout << endl << "m = ";
	cin >> m;
	cout << "a = ";
	cin >> a;

	// Request upper limit of prime numbers from user
	cout << "Searching all primes, p, from 2 to ";
	cin >> n;
	cout << endl;

	cout << "Please wait while processing data." << endl << endl;
	// For all numbers up to user n, attempt to add node to graph
	for (long long i = 2; i < n; i++)
	{
		while (MaxPrime(i) != i)
			i++;
		g.addNode(i);
	}
	
	// Sets the land value of all nodes in graph. Should be restructured to not run in main.
	g.setLand();
	// Calculates the semi-direct entrances table as well as setting values such as max-depth
	g.calcData();
	cout << "Please wait while important information is being written to the corresponing file." << endl << endl;
	// Outputs various data in text format to the relevent file in Stored Graphs directory. Does not currently check if file already exists before writing it.
	g.print(n);
	// Displays output to the console. Currently just entrances table.
	g.disp();

	
	// Hold shell's auto close for User to view data
	cout << endl << endl << endl;
	system("pause");
	
}

// Defines Iterator Function
long long op(long long in)
{
	long long out = in * m + a;
	return MaxPrime(out);
	
}

// Greatest Prime Factor Function
// Credit to alphacodingckills.com
long long MaxPrime(long long num) 
{
	long long CurrMaxPrime = -1;

	//If num is divisible by 2, store CurrMaxPrime
	//as 2. keep on dividing num until it is not 
	//divisible by 2. After each division, update 
	//num as num/2.
	if (num % 2 == 0) {
		CurrMaxPrime = 2;
		while (num % 2 == 0) {
			num = num / 2;
		}
	}
	
	//At this point, num must be odd. Starting with 
	//3 to square root of num , if divisible, divide 
	//and update num, and update CurrMaxPrime
	for (long long i = 3; i <= sqrt(num); i += 2) {
		while (num % i == 0) {
			CurrMaxPrime = i;
			num = num / i;
		}
	}

	//if the modified num is greater than 2, 
	//update the CurrMaxPrime with num 
	if (num > 2)
		CurrMaxPrime = num;

	return CurrMaxPrime;
}