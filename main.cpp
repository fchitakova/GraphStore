#include "Command.h"


int main(int argc,char*argv[])

{
	char*command = new char[32];

	Graph*graph = NULL;

	while (std::cin.getline(command, 32, '\n'))
	{
		readAndParseCommand(argv[1], command,graph);
	}
	



	delete[]command;
	command = NULL;
	return 0;
}