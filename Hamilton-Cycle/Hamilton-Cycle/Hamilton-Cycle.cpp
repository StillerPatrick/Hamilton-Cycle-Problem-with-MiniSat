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
vector<Edge> Edges ;

void initializeEdges(HandleFile*CurrentFile){
	int i = 0 ;  // Runtime Variable 
	int t = 0;
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
			Edge Edge_Buffer(Node(stoi(source),j),Node(stoi(destination),j+1));
			Edges.push_back(Edge_Buffer);
		}
		i++ ;
	}
	cout <<" Anzahl der Edges" <<Edges.size() << endl ;

}



int _tmain(int argc, _TCHAR* argv[])

{ 
	int a ;
	HandleFile CurrentFile("D:\\Patrick\\Studium\\4.Semester\\Forschungslinie\\Hamilton-Kreis\\graphs\\homer.col");
	cout << "Filepath" << CurrentFile.getPath() << endl ;
	cout << "Number of Edges " << CurrentFile.getNumOfEdges() << endl ;
	cout << "NumberofNodes " << CurrentFile.getNumOfNodes() << endl;
	initializeEdges(&CurrentFile);
	cout <<" Anzahl der Edges" <<Edges.size() << endl ;
	cin >> a ;
	
	return 0;
}

