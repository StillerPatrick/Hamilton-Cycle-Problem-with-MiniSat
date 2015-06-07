#include "HandleFile.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std ;

constexpr char GRAPHPROPERTIES = 'p';
constexpr char EDGE = 'e';

HandleFile::HandleFile(string FilePath)
{
    ifstream f; // file handle
    string s; // buffer string
    string properties; // propertie string

    f.open(FilePath.c_str());
	while(!f.eof()){
		getline(f,s);
		this->lines.push_back(s);
	}
	f.close();
	this->Path = FilePath ;
	//READFILE
	
    for (unsigned int i = 0; i < lines.size(); i++) {
        switch (lines[i][0]) {
            case GRAPHPROPERTIES    :   properties = lines[i];
            case EDGE               :   edges.push_back(lines[i]);
        }
	}

    cout << "Propertie line: '" << properties << '\'' << endl ;

    this->NumOfEdges = (int) edges.size() ;

    // cut the properties
    string cut =  properties.erase(0,properties.find(' ')+1);
	cut = properties.erase(0,properties.find(' ')+1);
	string Nodes = cut.substr(0,properties.find(' '));
	cut = cut.erase(0,properties.find(' ')+1);
	string EdgesS = cut.substr(0,properties.find(' ')+1);

    if (atoi(EdgesS.c_str()) != this->NumOfEdges){
        cout << "Datei wurde nicht richtig gelesen" << endl;
	}
	
    this->NumOfNodes = atoi(Nodes.c_str());
}

int HandleFile::getNumOfEdges(){
    return this->NumOfEdges;
}

int HandleFile::getNumOfNodes(){
    return this->NumOfNodes;
}

std::string HandleFile::getPath(){
	return this->Path; 
}
