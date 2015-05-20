#include "stdafx.h"
#include "HandleFile.h"
#include <sstream>
#include <vector>
#include <fstream>

using namespace std ;

HandleFile::HandleFile(string FilePath)
{
	int i=0; // runtime variable 

	ifstream f ; //file Handle
	string s ;
	f.open(FilePath,ios::in);
	while(!f.eof()){
		getline(f,s);
		this->lines.push_back(s);
	}
	f.close();
	this->Path = FilePath ;

	while(lines[i].c_str()[0] != 'p' && i <(int) lines.size()){
		i++ ;
	}

	cout << i << endl ;

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