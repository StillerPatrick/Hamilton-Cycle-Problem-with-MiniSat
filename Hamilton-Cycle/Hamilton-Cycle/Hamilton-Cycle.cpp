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
int n = 0;
int edge_count = 0;

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
	edge_count = numofedges;

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
	n = CurrentFile->getNumOfNodes();
}

void generateCNF() {
	stringstream alpha;
	int clause_count = 0;
	/* Performance optimization:
		- have constants for n * (n + 1), n + 1 and n * n
		- make all i, j, k register variables
		- nest the for-loops
		- do not access the Nodes array, work with indices directly
		- use something more efficient than stringstreams
	  FIRST: verify correctness using smaller graphs
	*/
	
	/* Each node must be visited at least once */
	alpha << "100" << endl;	/* Node 1 can be visited only at step 0 */
	alpha << "10" << Nodes[n].time << endl;	/* and at step n */
	for (int i = n + 1; i < n * (n + 1); i += n + 1) {
		for (int j = i + 1; j < i + n; j++) {
			alpha << Nodes[j].id << "0" << Nodes[j].time << " ";
		}
		alpha << endl;
		clause_count++;
	}
	
	/* Each node must be visited only once*/
	for (int i = 1; i < n; i++) {
		/* Node 1 cannot be visited at any step other than 0 or n */
		alpha << "-" << Nodes[i].id << "0" << Nodes[i].time << endl;
		clause_count++;
	}
	for (int i = n + 1; i < n * (n + 1); i += n + 1) {
		for (int j = i + 1; j < i + n - 1; j++) {
			for (int k = j + 1; k < i + n; k++) {
				alpha << "-" << Nodes[j].id << "0" << Nodes[j].time << " -" << Nodes[k].id << "0" << Nodes[k].time << endl;
				clause_count++;
			}
		}
	}

	/* At each step at least one node must be visited */
	// The clauses for step 0 and n visiting node 1 already exist
	for (int i = 1; i < n; i++) {
		for (int j = (n + 1) + i; j < n * (n + 1); j += n + 1) {
			alpha << Nodes[j].id << "0" << Nodes[j].time << " ";
		}
		alpha << endl;
		clause_count++;
	}
	
	/* At each step only one node must be visited */
	for (int i = n + 1; i < n * (n + 1); i += n + 1) {
		/* Steps 0 and n cannot visit any node other than 1 */
		alpha << "-" << Nodes[i].id << "0" << Nodes[i].time << endl;
		alpha << "-" << Nodes[i + n].id << "0" << Nodes[i + n].time << endl;
		clause_count += 2;
	}
	for (int i = 1; i < n; i++) {
		for (int j = (n + 1) + i; j < n * n; j += n + 1) {
			for (int k = 2 * (n + 1) + j; k < n * (n + 1); k += n + 1) {
				alpha << "-" << Nodes[j].id << "0" << Nodes[j].time << " -" << Nodes[k].id << "0" << Nodes[k].time << endl;
				clause_count++;
			}
		}
	}

	/* Edges */
	for (int i = 0; i < edge_count; i++) {
		alpha << "-" << Edges[i].sourceID << "0" << Edges[i].sourceTime << " " << Edges[i].destinationID << "0" << Edges[i].destinationTime << endl;
		clause_count++;
	}

	ofstream file;
	file.open("cnf");
	file << "p cnf " << n * (n + 1) << " " << clause_count << endl << alpha.str();
	file.close();
	//cout << alpha.str();
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
	generateCNF();
	int minisatreturn ;
	minisatreturn =CallMiniSat("..\\test.in","..\\test.out");
	cout << "MiniSAt return: " << minisatreturn << endl ;
	system("PAUSE");
	
	return 0;
}

