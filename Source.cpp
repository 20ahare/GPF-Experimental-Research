#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdlib.h>

using namespace std;

int m;
int a;


int op(int in);
int MaxPrime(int num);


class Graph
{
private:

	struct Node
	{
		int key = -1;
		Node* next = nullptr;
		bool isLast = true;
		int depth = 0;
		int Land = -1;
	};

	// 2d list of nodes [key,ptr]

	vector<int> vals;
	vector<Node*> pts;

	vector<Node*> adj;
	vector<Node*> cycleList;

public:

	
	// Takes in a vector that provides a list of weights corresponding to the existing nodes  
	Node* addNode(int val)
	{
		for (int i = 0; i < vals.size(); i++)
		{
			if (val == vals[i])
				return pts[i];
		}
		Node* newnode = new Node;
		vals.push_back(val);
		pts.push_back(newnode);
		newnode->key = val;
		newnode->next = addNode(op(val));
		newnode->depth = 0;
		return newnode;
	}
	// Displays accrued adjascency matrix in triangle array form
	void disp()
	{
		/*for (int i = 0; i < vals.size(); i++)
		{
			cout << vals[i] << ">" << pts[i]->next->key << endl;
		}*/

		vector<int>done = {};
		for (int i = 0; i < adj.size(); i++)
		{
			Node* cur = adj[i];
			while (find(done.begin(), done.end(), cur -> key) == done.end())
			{
				/*	
				cout << cur->key << ">";
				done.push_back(cur->key);
				cur = cur->next;
				*/

				cout << cur->depth << ">";
				done.push_back(cur->key);
				cur = cur->next;
				
				
			}
			cout << cur->depth << endl << endl;
	
		}

		cout << "number of nodes: " << vals.size()+1;
	}
	void cyclGen()
	{
		for (int i = 0; i <= pts.size(); i++)
		{
			Node* start = pts[i];
			if (start->depth <= start->next->depth)
				while (start->depth != 0)
				{
					cycleList.push_back(start);
					start->depth = 0;
					start = start->next;
					
					
				}
		}
	}
	void depth()
	{
		//Recursively add 1 to value of next->depth
		for (int i = 0; i < pts.size(); i++)
		{
			if (pts[i]->isLast)
			{
			}
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
		for (int i = 0; i < vals.size(); i++)
		{
			output << vals[i] << ">" << pts[i]->next->key << endl;
			
		}
		output.close();
	}

};

int main(int argc, char* argv[])
{
	Graph g;

	int n;

	// Request uper limit of prime numbers from user

	cout << "Iterator is in the form GPF(p * m + a) = p'" << endl << "Please enter values for m and a respectively:";
	cout << endl << "m = ";
	cin >> m;
	cout << "a = ";
	cin >> a;


	cout << "Search all primes, p, from 2 to ";
	cin >> n;
	cout << endl;

	string M = to_string(m);
	string A = to_string(a);
	string N = to_string(n);
	string path = " \"m = " + to_string(m) + ", a = " + to_string(a) + ", n = " + to_string(n) + ".txt\"";
	string exec = "Graph_Display.py";
	exec = exec + " " + path;
	

		for (int i = 2; i <= n; i++)
		{
			while (MaxPrime(i) != i)
				i++;
			g.addNode(i);
		}
		g.adjList();
		g.cyclGen();
		g.disp();

		g.print(n);


	cout << endl << endl << endl;


	LPSTR args = const_cast<char*>(exec.c_str());

	
	// Method of executing new process via system borrowed from https://stackoverflow.com/questions/3906515/cannot-convert-from-stdstring-to-lpstr
	
	

	

	/*
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	CreateProcess("Graph_Display.exe",
		args,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&info,
		&processInfo
	);

	
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);
	system("pause");
	*/
	
}

int op(int in)
{
	int out = in * m + a;
	return MaxPrime(out);
	
}

// Credit to alphacodingckills.com
int MaxPrime(int num) 
{
	int CurrMaxPrime = -1;

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
	for (int i = 3; i <= sqrt(num); i += 2) {
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