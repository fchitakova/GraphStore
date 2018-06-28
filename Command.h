#pragma once
#ifndef COMMAND_H
#define COMMAND_H


#include "Graph.h"
#include <windows.h>

struct Command
{

	static void CREATE_GRAPH(const char*path,const char*id, bool isDirected,Graph*&currentGraph);
	static void USE_GRAPH(const char*path, const char*id, Graph*&currentGraph);
	static void DELETE_GRAPH(const char*path, const char*id, Graph*&currenGraph);
	static void CREATE_NODE(const char*id,Graph*&currentGraph);
	static void DELETE_NODE(const char*id, Graph*&currentGraph);
	static void CREATE_ARC(const char*startNode, const char*endNode,int weight, Graph*&currentGraph);
	static void DELETE_ARC(const char*startNode, const char*endNode,Graph*&currentGraph);

};

//if graph is in the directory returns the full path of the filename name 
//else deletes the copyPath string and returns NULL pointer
 const char* searchForGraph(const char*,const char*);

 void readAndParseCommand(const char*path,const char*commandLine,Graph*&currentGraph);


#endif


