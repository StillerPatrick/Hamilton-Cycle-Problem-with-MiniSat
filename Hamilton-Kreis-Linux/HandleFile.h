/* HandleFile gibt die Informationen des Aktuellen Graphen wieder */

#pragma once
#ifndef H_HANDLEFILE
#define H_HANDLEFILE 
#include <vector>
#include <string>
#include <iostream>
class HandleFile
{
public:
	HandleFile(std::string Path);
	HandleFile(void);
	virtual ~HandleFile(void);
	std::vector<std::string> lines ;
	std::vector<std::string> edges ;
	std::string getPath() ;
	int getNumOfNodes();
	int getNumOfEdges();

	

private: 
	int NumOfNodes;
	int NumOfEdges;
	std::string Path ;

};

#endif 

