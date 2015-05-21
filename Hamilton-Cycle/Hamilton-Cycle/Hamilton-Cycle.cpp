// Hamilton-Cycle.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Node.h"
#include "HandleFile.h"
#include <sstream>
#include <vector>
#include <fstream>


using namespace std ;


vector<string> lines ;

void initializeEdges(HandleFile*CurrentFile){
	int i = 0 ;  // Runtime Variable 
	int t = 0;
	while(i < CurrentFile->getNumOfEdges()){
	// 1st Cut the String
		string source ;
		string destination; 
		string buffer ;
		buffer = CurrentFile->edges[i] ;
		cout << buffer <<endl ;
		i++ ;
	}

}



int _tmain(int argc, _TCHAR* argv[])

{ 
	int a ;
	HandleFile CurrentFile("D:\\Patrick\\Studium\\4.Semester\\Forschungslinie\\Hamilton-Kreis\\graphs\\homer.col");
	cout << "Filepath" << CurrentFile.getPath() << endl ;
	cout << "Number of Edges " << CurrentFile.getNumOfEdges() << endl ;
	cout << "NumberofNodes " << CurrentFile.getNumOfNodes() << endl;
	initializeEdges(&CurrentFile);
	cin >> a ;
	
	return 0;
}

