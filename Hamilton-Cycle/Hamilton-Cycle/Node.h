#pragma once
class Node
{
private:
	int id  ;
	int time;
	std::string output ;
	
public:
	Node(int id , int time);
	~Node(void);
	int  getID() ;
	int  getTime();
	std::string getOutput();
};

