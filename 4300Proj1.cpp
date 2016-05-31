// 4300Proj1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <queue>
#include <iostream>

//struct to hold the composition of a side
struct comp{	int m; //number of missionaries
				int c; //number of cannibals
			};
struct node{	char side; //determines which side the node is currently on for expansion
				comp start; //holds values of missionaries and cannibals at starting point
				comp finish; //holds values of missionaries and cannibals at finishing point
				node* parent; // holds the parent of this node
			};
using namespace std;
void searchStates(node, comp *);
node * newNode(node*, comp);
comp cAdd(comp, comp);
comp cSub(comp, comp);
bool negNode(node *);
void printNode(node *);
int main()
{
	node initialState; //node to hold problem starting point
	initialState.parent = NULL;
	initialState.start.m = 3;
	initialState.start.c = 3;
	initialState.finish.m = 0;
	initialState.finish.c = 0;
	comp States[5]; //holds array of possible changes to state
	States[0].m = 2;
	States[0].c = 0;
	States[1].m = 0;
	States[1].c = 2;
	States[2].m = 1;
	States[2].c = 1;
	States[3].m = 1;
	States[3].c = 0;
	States[4].m = 0;
	States[4].c = 1;
	initialState.side = 's';
	searchStates(initialState, States);

	return 0;
}

void searchStates(node iState, comp* st) //function to expand and keep track of the frontier. Parameters are the problem starting state and possible state changes.
{
	vector<node*> qpNodes; //used to store parents of all nodes from goal state, for printing
	queue<node*> frontier; //frontier of nodes to expand
	queue<node*> nfrontier; //next frontier, holds nodes generated by the current frontier expansion
	vector<queue<node*> > level; //stores each frontier based on its level
	bool goal = false; //hold value to determine if goal state has been reached
	comp *state = st; //pointer to array of state changes
	node is = iState; //initial state of the problem
	node *root = &is; //root node of tree
	node *current = root; //current node being expanded
	node *temp = NULL; //hold expansion of current node to be evaluated and pushed into nfrontier
	int vlev = 0; //tracks the level
	nfrontier.push(root); //put root into next frontier
	while(goal == false)
	{
		if(!goal)
		{
			level.push_back(nfrontier); //store next frontier level
			while(nfrontier.empty() == false) nfrontier.pop(); //clear next frontier
			frontier = level[vlev]; //set frontier to next frontier for exploration
			vlev++; //increment level
		}
		while(!frontier.empty() && !goal)
		{
			current = frontier.front(); //set node of first frontier for expansion
			frontier.pop();
			for(int i=0; i<5; i++) //iterate through the 5 possible state changes for possible expansion
			{
				temp = newNode(current, state[i]); //expand node and store in temp
				if(temp != NULL) 
				{
					if(temp != root) nfrontier.push(temp); //push onto next frontier if valid
					if(temp->finish.c == 3 && temp->finish.m == 3) goal = true; //check if goal state achieved
				}
				if(goal) break;
			}
		}
		if(goal)
		{
			cout << "Goal found at level " << vlev << endl; //print level goal was found at
			//trace back and store all parent nodes of goal state and print them
			while(temp != NULL)
			{
				qpNodes.push_back(temp);
				temp = temp->parent;
			}
			while(!qpNodes.empty())
			{
				printNode(qpNodes.back());
				qpNodes.pop_back();
			}
		}
	}

}

node * newNode(node *state, comp sChange) //function to expand a node
{
	bool scheck = true; //check balance of starting side
	bool fcheck = true; //check balance of finishing side
	bool ncheck = true; //check for negative values
	node* nNode = new node; //expansion of node given in parameter
	nNode->parent = state; //set the parent of the node being expanded
	if(state->side == 's') //determine the state change if the parent is at the starting side
	{
		nNode->side = 'f';
		nNode->start = cSub(state->start, sChange);
		nNode->finish = cAdd(state->finish, sChange);
	}
	else //determine the state change if the parent is at the finishing side
	{
		nNode->side = 's';
		nNode->start = cAdd(state->start, sChange);
		nNode->finish = cSub(state->finish, sChange);
	}
	if(negNode(nNode)) ncheck = false; //check for node with negative value
	if(nNode->start.m != 0) //check node balance on starting side
	{
		if(nNode->start.m < nNode->start.c) scheck = false;
	}
	if(nNode->finish.m != 0) //check node balance on finishing side
	{
		if(nNode->finish.m < nNode->finish.c) fcheck = false;
	}
	if(scheck && fcheck && ncheck)return nNode; //return node if conditions met
	//delete node and return null if invalid state
	delete [] nNode;
	return NULL;
}

comp cAdd(comp a, comp b) //function to add node composition to state change
{
	comp temp;
	temp.m = a.m + b.m;
	temp.c = a.c + b.c;
	return temp;
}

comp cSub(comp a, comp b) //function to subtract state change from node composition
{
	comp temp;
	temp.m = a.m - b.m;
	temp.c = a.c - b.c;
	return temp;
}

bool negNode(node *n) //function to determine negative value in node
{
	if(n->finish.m < 0 || n->finish.c < 0 || n->start.m < 0 || n->start.c < 0) return true;
	else return false;
}

void printNode(node *n) //function to print a node
{
	cout << endl;
	cout << "Side: " << n->side << endl;
	cout << "Start: " << endl;
	cout << "M: " << n->start.m << endl;
	cout << "C: " << n->start.c << endl;
	cout << "Finish: " << endl;
	cout << "M: " << n->finish.m << endl;
	cout << "C: " << n->finish.c << endl;
}