// Hamilton-Cycle.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Node.h"
#include "Edge.h"
#include "HandleFile.h"
#include <sstream>
#include <vector>
#include <fstream>


using namespace std ;


vector<string> lines ;
struct Edge* Edges = NULL;

void initializeEdges(HandleFile*CurrentFile){
	int i = 0 ;  // Runtime Variable 
	int numofedges = 0;
	Edges = (struct Edge*) malloc(sizeof(struct Edge)*CurrentFile->getNumOfEdges() * CurrentFile->getNumOfNodes());
	while(i < CurrentFile->getNumOfEdges()){
	// 1st Cut the String
		string source ;
		string destination; 
		string buffer ;
		
		buffer = CurrentFile->edges[i] ;
		buffer.erase(0,buffer.find(' ')+1);
		source = buffer.substr(0,buffer.find(' '));
		buffer.erase(0,buffer.find(' ')+1);
		destination = buffer.substr(0,buffer.find(' '));
		//cout << "."<<source << "." << endl ;
		//cout << "."<<destination<<"."<< endl;
		for (int j = 0 ; j < CurrentFile->getNumOfNodes() ; j++){
			Edges[numofedges].sourceID = stoi(source);
			Edges[numofedges].sourceTime = j ;
			Edges[numofedges].destinationID = stoi(destination);
			Edges[numofedges].destinationTime= j+1;
			numofedges = numofedges + 1;
		}
		i++ ;
	}
	
	cout << numofedges << endl ;

}



int _tmain(int argc, _TCHAR* argv[])

{ 
	int a ;
	int i = 0 ;

	HandleFile CurrentFile("..\\graphs\\homer.col"); //Change your Path
	cout << "Filepath" << CurrentFile.getPath() << endl ;
	cout << "Number of Edges " << CurrentFile.getNumOfEdges() << endl ;
	cout << "NumberofNodes " << CurrentFile.getNumOfNodes() << endl;
	initializeEdges(&CurrentFile);
	for(i=0; i < CurrentFile.getNumOfEdges()*CurrentFile.getNumOfNodes(); i++){
		cout << Edges[i].sourceID <<Edges[i].sourceTime << endl ;
		cout << Edges[i].destinationID <<Edges[i].destinationTime << endl ;
	}
	cin >> a ;
	
	return 0;
}

