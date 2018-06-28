#include "Command.h"

const char* searchForGraph(const char *path,const char*id)
{
    char*copyPath = new char[MAX_PATH+3];
	
	strcpy_s(copyPath,MAX_PATH + 3,path);
	strcat_s(copyPath, MAX_PATH + 3, "\\*");

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(copyPath, &ffd);

	do
	{
		if (hFind == INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
			return NULL;
		}
		else
		{
			if (strcmp(ffd.cFileName, id) == 0)
			{
				strcpy_s(copyPath, MAX_PATH + 3, path);
				strcat_s(copyPath, MAX_PATH + 3, "\\" );
				strcat_s(copyPath, MAX_PATH + 3, id);
				FindClose(hFind);
				return copyPath;
			}
		}
	} while (FindNextFile(hFind, &ffd)!=0);
	
	FindClose(hFind);
	if(copyPath)
	    delete[]copyPath;
	return NULL;

}

void Command::CREATE_GRAPH(const char*path,const char * id, bool isDirected,Graph*&currentGraph)
{
	char*fullName = new char[64];
	strcpy_s(fullName, 64, id);
	strcat_s(fullName, 64, ".txt");

	const char*result = searchForGraph(path, fullName);
	
	if (!result)
	{
		char*filePath = new char[MAX_PATH + 3];
		strcpy_s(filePath, MAX_PATH + 3, path);
		strcat_s(filePath, MAX_PATH + 3, "\\");
		strcat_s(filePath, MAX_PATH + 3, fullName);

		std::fstream newGraph(filePath);
		newGraph.open(filePath, std::fstream::out);
        
		if (isDirected)
			newGraph.write("directed",8);
		else
			newGraph.write("undirected",10);

		delete[]filePath;
		filePath = NULL;
		newGraph.close();


		//IF THERE IS CURRENT GRAPH LOADED IN MEMORY IT IS SAVED TO A FILE AND THEN DELETED FROM HEAP MEMORY
		if (currentGraph)
		{
			currentGraph->saveGraphToAFile(path);
			delete currentGraph;
		}
        
		currentGraph = new Graph(id, isDirected);
		
		delete[]fullName;
		fullName = NULL;

		return;
	}



	delete[]fullName;
	fullName = NULL;
	 
	delete[]result;
	result = NULL;

	std::cout << "The graph " << id << " already exists!\n";
}

void Command::USE_GRAPH(const char * path, const char * id, Graph *&currentGraph)
{

	char*filePath = new char[MAX_PATH + 3];
	char*fullName = new char[64];

	strcpy_s(fullName, 64, id);
	strcat_s(fullName, 64, ".txt");
	
	strcpy_s(filePath, MAX_PATH + 3, path);
	strcat_s(filePath, MAX_PATH + 3, "\\");
	strcat_s(filePath, MAX_PATH + 3, id);
	strcat_s(filePath, MAX_PATH + 3, ".txt");
	
	if (searchForGraph(path,fullName ))
	{
		if (currentGraph)
		{
			currentGraph->saveGraphToAFile(path);
			delete currentGraph;
		}
		currentGraph = new Graph(id);
		currentGraph->loadGraph(filePath, id);
		return;
	}

	std::cout << "Graph " << id << " does not exists!\n";
	
	delete[]filePath;
	delete[]fullName;

}

void Command::DELETE_GRAPH(const char * path, const char * id, Graph *& currenGraph)
{
	char*fullName1 = new char[64];
	strcpy_s(fullName1, 64, id);
	strcat_s(fullName1, 64, ".txt");

	//deleting current graph
	if (currenGraph && strcmp(currenGraph->graphID, id) == 0)
	{
		delete currenGraph;
		currenGraph = NULL;
	}
	//if graph file is in the storage
	else if (searchForGraph(path, fullName1))
	{
		char*fullName = new char[MAX_PATH + 3];
		strcpy_s(fullName, MAX_PATH + 3, path);
		strcat_s(fullName, MAX_PATH + 3, "\\");
		strcat_s(fullName, MAX_PATH + 3, fullName1);
		remove(fullName);
		delete[]fullName;
		fullName = NULL;
	}
	else
		std::cout << "Graph " << id << " does not exists\n";
	
	delete[]fullName1;
	fullName1 = NULL;
}

void Command::CREATE_NODE(const char * id, Graph *& currentGraph)
{
	//this command works only if there is loadedgraph
	if (currentGraph)
		currentGraph->createNode(id, false);
	else
		std::cout << "Please first create or load any graph!\n";
}

void Command::DELETE_NODE(const char * id, Graph *& currentGraph)
{
	//this command works only if there is loadedgraph
	if (currentGraph)
		currentGraph->deleteNode(id);
	else
		std::cout << "Please first create or load any graph!\n";

	currentGraph->printAllNodesSucc(std::cout);

}

void Command::CREATE_ARC(const char * startNode, const char * endNode, int weight, Graph *& currentGraph)
{
	currentGraph->createArc(startNode, endNode, weight);
}

void Command::DELETE_ARC(const char * startNode, const char * endNode, Graph *& currentGraph)
{
	currentGraph->deleteArc(startNode, endNode);
}


void readAndParseCommand(const char*path, const char*commandLine, Graph*&currentGraph)
{
	if (commandLine[0] == 'C' && commandLine[7] == 'G')
	{
		char*id = new char[MAX_PATH];
		bool option = false;

		int inx = 0;
		int i;
		int length = strlen(commandLine);
		for (i = 13; commandLine[i] != ' ' && i < length; ++i)
			id[inx++] = commandLine[i];
		id[inx] = '\0';

		++i;
		if (commandLine[i] == 'd')
			option = true;

		Command::CREATE_GRAPH(path, id, option, currentGraph);

	}

	else if (commandLine[0] == 'U' && commandLine[4] == 'G')
	{

		char*graphID = new char[32];
		int inx = strlen("USE GRAPH "), inx2 = 0;

		for (int i = inx; commandLine[i] != '\0'; ++i)
			graphID[inx2++] = commandLine[i];

		graphID[inx2] = '\0';

		char*fullName = new char[64];
		strcpy_s(fullName, 64, graphID);
		strcat_s(fullName, 64, ".txt");

		Command::USE_GRAPH(path, graphID, currentGraph);



		delete[]graphID;
		graphID = NULL;
		delete[]fullName;
		fullName = NULL;
	}

	else if (commandLine[0] == 'D' && commandLine[7] == 'G')
	{
		char*graphID = new char[32];
		
		int inx = strlen("DELETE GRAPH "), inx2 = 0;
		
		for (int i = inx; commandLine[i] != '\0'; ++i)
			graphID[inx2++] = commandLine[i];
		graphID[inx2] = '\0';
	
		Command::DELETE_GRAPH(path, graphID, currentGraph);
		
		delete[]graphID;
		graphID = NULL;
	}

	else if (commandLine[0] == 'C' && commandLine[7] == 'N')
	{
		char*nodeID = new char[32];

		int inx = strlen("CREATE NODE "), inx2 = 0;

		for (int i = inx; commandLine[i] != '\0'; ++i)
			nodeID[inx2++] = commandLine[i];
		nodeID[inx2] = '\0';

		Command::CREATE_NODE(nodeID, currentGraph);
		delete[]nodeID;
		nodeID = NULL;
	}
	
	else if (commandLine[0] == 'D' && commandLine[7] == 'N')
	{
		char*nodeID = new char[32];

		int inx = strlen("DELETE NODE "), inx2 = 0;

		for (int i = inx; commandLine[i] != '\0'; ++i)
			nodeID[inx2++] = commandLine[i];
		nodeID[inx2] = '\0';

		Command::DELETE_NODE(nodeID, currentGraph);
		delete[]nodeID;
		nodeID = NULL;
	}

	else if (commandLine[0] == 'C' && commandLine[7] == 'A')
	{
		char*nodeId1 = new char[32];
		char*nodeId2 = new char[32];
		char*weight = new char[32];
		int weightNum = 1;

		int inx = strlen("CREATE ARC "), inx2 = 0;

		for (; commandLine[inx] != ' '; ++inx2, ++inx)
			nodeId1[inx2] = commandLine[inx];
		nodeId1[inx2] = '\0';

		++inx;
		inx2 = 0;
		
		int length = strlen(commandLine);

		for (; commandLine[inx] != ' ' && inx <length; ++inx2, ++inx)
			nodeId2[inx2] = commandLine[inx];
		nodeId2[inx2] = '\0';

		
		if (++inx < length)
		{
			inx2 = 0;
			for (; commandLine[inx] != ' ' && inx<length; ++inx2, ++inx)
				weight[inx2] = commandLine[inx];
			weight[inx2] = '\0';
			weightNum = atoi(weight);
		}

		Command::CREATE_ARC(nodeId1, nodeId2, weightNum, currentGraph);
		

		delete[]nodeId1;
		nodeId1 = NULL;
		delete[]nodeId2;
		nodeId2 = NULL;
		if (weight)
		{
			delete[]weight;
			weight = NULL;
		}

	}

	else if (commandLine[0] == 'D' && commandLine[7] == 'A')
	{
		char*nodeId1 = new char[32];
		char*nodeId2 = new char[32];

		int inx = strlen("DELETE ARC "), inx2 = 0;

		for (; commandLine[inx] != ' '; ++inx2, ++inx)
			nodeId1[inx2] = commandLine[inx];
		nodeId1[inx2] = '\0';

		++inx;
		inx2 = 0;

		int length = strlen(commandLine);

		for (; commandLine[inx] != ' ' && inx <length; ++inx2, ++inx)
			nodeId2[inx2] = commandLine[inx];
		nodeId2[inx2] = '\0';


		Command::DELETE_ARC(nodeId1, nodeId2, currentGraph);


		delete[]nodeId1;
		nodeId1 = NULL;
		delete[]nodeId2;
		nodeId2 = NULL;

	}
}



