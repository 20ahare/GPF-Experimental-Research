#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdlib.h>

using namespace std;

int m;
int a;


long long op(long long in);
long long MaxPrime(long long num);


class Graph
{
private:

	struct Node
	{
		long long key = -1;
		Node* next = nullptr;
		bool isLast = true;
		int depth = -1;
		int land = -1;
	};


	// unnecessary vector, stores key values in same order as vals, can be taken out of functions and rewritten as time allows
	vector<long long> vals;

	vector<Node*> pts;

	vector<Node*> adj;
	vector<vector<int>> entrances;
	vector<int> cycles = {};
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
		/*
		vector<int>done = {};
		for (int i = 0; i < adj.size(); i++)
		{
			Node* cur = adj[i];
			while (find(done.begin(), done.end(), cur -> key) == done.end())
			{

				cout << cur->key << ">";
				done.push_back(cur->key);
				cur = cur->next;
				
			}
			cout << cur->depth << endl << endl;
		
		}
		cout << "number of nodes: " << pts.size() << endl;

		// Display "Table" of land and depth values
		cout << "Depth and Land Values for each seed: " << endl;
		for (int j = 0; j < pts.size(); j++)
		{
			cout << pts[j]->key << " | " << pts[j]->depth << " | " << pts[j]->land << endl;
		}
		cout << endl << endl;
		*/
	
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

	void adjList()
	{
		for (int i = 0; i < pts.size(); i++)
		{
			pts[i]->next->isLast = false;
		}
		for (int j = 0; j < pts.size(); j++)
		{
			if (pts[j]->isLast == true)
			{
				adj.push_back(pts[j]);
			}
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



int main(int argc, char* argv[])
{
	Graph g;

	long long n;

	// Request uper limit of prime numbers from user

	cout << "Iterator is in the form GPF(p * m + a) = p'" << endl<< endl << "Please enter values for m and a respectively:";
	cout << endl << "m = ";
	cin >> m;
	cout << "a = ";
	cin >> a;


	cout << "Searching all primes, p, from 2 to ";
	cin >> n;
	cout << endl;

	cout << "Please wait while processing data." << endl << endl;

	for (long long i = 2; i < n; i++)
	{
		while (MaxPrime(i) != i)
			i++;
		g.addNode(i);
	}
	// Populates the adjascency list for display. Should be restructured to not run in main.
	g.adjList();
	// Sets the land value of all nodes in graph. Should be restructured to not run in main.
	g.setLand();
	// Calculates the semi-direct entrances table as well as setting values such as max-depth
	g.calcData();
	cout << "Please wait while important information is being written to the corresponing file." << endl << endl;
	// Outputs various data in text format to the relevent file in Stored Graphs directory. Does not currently check if file already exists before writing it.
	g.print(n);
	// Displays output to the console. Currently just entrances table.
	g.disp();

	

	cout << endl << endl << endl;


	system("pause");
	
}

long long op(long long in)
{
	long long out = in * m + a;
	return MaxPrime(out);
	
}

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