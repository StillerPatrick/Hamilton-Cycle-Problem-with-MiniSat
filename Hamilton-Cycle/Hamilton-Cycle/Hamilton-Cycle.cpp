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



int _tmain(int argc, _TCHAR* argv[])

{ 
	int a ;
	HandleFile CurrentFile("D:\\Patrick\\Studium\\4.Semester\\Forschungslinie\\Hamilton-Kreis\\graphs\\homer.col");
	cout << "Filepath" << CurrentFile.getPath() << endl ;
	cout << "Number of Edges " << CurrentFile.getNumOfEdges() << endl ;
	cout << "NumberofNodes " << CurrentFile.getNumOfNodes() << endl;
	cin >> a ;
	
	return 0;
}

