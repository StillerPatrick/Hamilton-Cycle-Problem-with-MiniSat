#include "stdafx.h"
#include "HandleFile.h"
#include <sstream>
#include <fstream>

using namespace std ;

#define GRAPHPROPERTIES 'p' 
#define EDGE 'e'

HandleFile::HandleFile(void){

}
HandleFile::HandleFile(string FilePath)
{
	int i=0; // runtime variable 

	ifstream f ; //file Handle
	string s ; //BufferString  ;
	string properties; // PropertieString
	f.open(FilePath,ios::in);
	while(!f.eof()){
		getline(f,s);
		this->lines.push_back(s);
	}
	f.close();
	this->Path = FilePath ;
	//READFILE
	
	while(i < (int) lines.size()){
		if(lines[i].c_str()[0] == GRAPHPROPERTIES){
			properties = lines[i] ;
		}
		if(lines[i].c_str()[0] == EDGE)
		{
			edges.push_back(lines[i]);
		}

		i++ ;
	}

	cout << "PropertieLine :" << properties <<endl ;


	this ->NumOfEdges = (int) edges.size() ;

	//Cut The Properties 

	string cut =  properties.erase(0,properties.find(' ')+1);;
	cut = properties.erase(0,properties.find(' ')+1);
	string Nodes = cut.substr(0,properties.find(' '));
	cut = cut.erase(0,properties.find(' ')+1);
	string EdgesS = cut.substr(0,properties.find(' ')+1);

	if ( stoi(EdgesS) != this->NumOfEdges){
		cout << "File wurde nicht richtig gelesen"<< endl ;
	}
	
	this->NumOfNodes = stoi(Nodes);
}


HandleFile::~HandleFile(void)
{
}

int HandleFile::getNumOfEdges(){
	return this->NumOfEdges ;
}

int HandleFile::getNumOfNodes(){
	return this->NumOfNodes ;
}

std::string HandleFile::getPath(){
	return this->Path; 
}