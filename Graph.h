#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include <fstream>
#include <string>

struct Graph
{
	char*graphID;
	//the top and end of the list of graph nodes
	Node*top;
	Node*end;

	bool isDirected;

	Graph(const char*graphID,bool isDirected=false);
	Graph(const Graph&other)=delete;
	Graph&operator=(const Graph&other)=delete;
	~Graph();


	//saves the graph in a file before making another graph current 
	void saveGraphToAFile(const char*filePath);

	//loads existing graph from a file
	void loadGraph(const char*filename,const char*id);

	//if createNode is called while loading graph from a file message for existence is not displayed
	void createNode(const char*id,bool isForLoading);

	/*returns pointer to the previous node or NULL if searched one does not exists in this graph 
	 if searched is the top returns pointer to the top*/
	Node* searchForNode(const char*id);

	//deletes the node from Node list and if it is successor of other from its Successor lists 
	void deleteNode(const char*id);

	void createArc(const char*id1, const char*id2, int weight = -1);

	void deleteArc(const char*id1, const char*id2);

	void deleteGraph();

	void printAllNodesSucc(std::ostream&os);

};


#endif
