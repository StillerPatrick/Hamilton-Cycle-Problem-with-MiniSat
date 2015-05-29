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
struct Node* Nodes = NULL;

int CallMiniSat(string inputpath , string outputpath){
	stringstream systemcall ;
	systemcall << "Minisat.exe " << inputpath << " " << outputpath ;
	int result = system(systemcall.str().c_str());
	return result ;
}
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
	
	cout <<"Num of initializes Edges :" << numofedges << endl ;

}

void intitializeNodes(HandleFile* CurrentFile){
	Nodes = (struct Node*) malloc(sizeof(struct Node)*CurrentFile->getNumOfNodes() * (CurrentFile->getNumOfNodes()+1));
	int count = 0 ;
	int time = CurrentFile->getNumOfNodes()+1 ;
	cout << "Time" << time << endl ;
	// i is the runtime for the node id 
	// j is the runtime for the node time
	for(int i = 1; i < CurrentFile->getNumOfNodes()+1 ; i++)
	{
		for(int j = 0; j < CurrentFile->getNumOfNodes()+1; j++)
		{
			Nodes[count].id = i ;
			Nodes[count].time = j ;
			count++ ;
		}
	}
	cout << "Num of initializes Nodes :" << count << endl ;
}


int _tmain(int argc, _TCHAR* argv[])

{ 
	int i = 0 ;

	HandleFile CurrentFile("..\\graphs\\homer.col"); //Change your Path
	cout << "Filepath" << CurrentFile.getPath() << endl ;
	cout << "Number of Edges " << CurrentFile.getNumOfEdges() << endl ;
	cout << "NumberofNodes " << CurrentFile.getNumOfNodes() << endl;
	initializeEdges(&CurrentFile);
	intitializeNodes(&CurrentFile);
	/*
	for(i=0; i < CurrentFile.getNumOfEdges()*CurrentFile.getNumOfNodes(); i++){
		cout << Edges[i].sourceID <<Edges[i].sourceTime << endl ;
		cout << Edges[i].destinationID <<Edges[i].destinationTime << endl ;
	}*/

	// 
	//Example for simple MiniSAT call
	// "test.in" is a example file 
	int minisatreturn ;
	minisatreturn =CallMiniSat("..\\test.in","..\\test.out");
	cout << "MiniSAt return: " << minisatreturn << endl ;
	system("PAUSE");
	
	return 0;
}

