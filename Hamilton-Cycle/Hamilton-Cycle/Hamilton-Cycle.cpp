// Hamilton-Cycle.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Node.h"
#include <sstream>
#include <vector>
#include <fstream>


using namespace std ;

vector<string> lines ;

void readfile(string filename){
	ifstream f ; //file Handle
	string s ;
	f.open(filename,ios::in);
	while(!f.eof()){
		getline(f,s);
		lines.push_back(s);
	}
	f.close();
}

int _tmain(int argc, _TCHAR* argv[])

{ 
	int a ;
    readfile("D:\\Patrick\\Studium\\4.Semester\\Forschungslinie\\Hamilton-Kreis\\graphs\\homer.col");
	 for (std::vector<string>::iterator it = lines.begin() ; it != lines.end(); ++it){
		cout << *it << endl ;
	}

	cin >> a ;
	
	return 0;
}

