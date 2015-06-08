/* HandleFile gibt die Informationen des Aktuellen Graphen wieder */

#pragma once
#ifndef H_HANDLEFILE
#define H_HANDLEFILE 
#include <vector>
#include <string>
#include <iostream>
class HandleFile {
public:
    HandleFile(const std::string& Path);
	std::vector<std::string> lines ;
	std::vector<std::string> edges ;
	std::string getPath() ;
    unsigned int getNumOfNodes();
    unsigned int getNumOfEdges();

private: 
    unsigned int NumOfNodes;
    unsigned int NumOfEdges;
    std::string Path;
};

#endif 

