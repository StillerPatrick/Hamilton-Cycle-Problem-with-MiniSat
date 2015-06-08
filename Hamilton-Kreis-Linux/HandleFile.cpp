#include "HandleFile.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

using namespace std ;

constexpr char GRAPHPROPERTIES = 'p';
constexpr char EDGE = 'e';

HandleFile::HandleFile(const string& FilePath) : Path{FilePath} {
    ifstream f(FilePath, std::ifstream::in); // file handle
    string properties; // propertie string
    string line; // buffer string

    /* read lines from file to 'lines' */
    while(f.good()){
        switch (f.peek()) {
            case GRAPHPROPERTIES    :   getline(f, line); properties = line; break;
            case EDGE               :   getline(f, line); edges.push_back(line); break;
        }
	}

	f.close();

    cout << "c Propertie line: '" << properties << '\'' << endl;

    this->NumOfEdges = edges.size();

    /* cut the properties */
    istringstream prop(properties);
    prop.ignore(7); // ignore "p edges"
    unsigned int num_nodes;
    unsigned int num_edges;
    prop >> std::skipws >> num_nodes;
    prop >> std::skipws >> num_edges;
    if (num_edges != this->NumOfEdges){
        cerr << "Datei wurde nicht richtig gelesen" << endl;
	}
	
    this->NumOfNodes = num_nodes;
}

unsigned int HandleFile::getNumOfEdges(){
    return this->NumOfEdges;
}

unsigned int HandleFile::getNumOfNodes(){
    return this->NumOfNodes;
}

std::string HandleFile::getPath(){
	return this->Path; 
}
