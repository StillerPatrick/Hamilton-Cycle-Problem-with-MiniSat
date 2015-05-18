#pragma once
#ifndef NODE_H 
#define NODE_H
class Node
{
private:
	int id  ;
	int time;
	std::string output ;
	
public:
	Node(int id , int time);
	Node(void);
	~Node(void);
	int  getID() ;
	int  getTime();
	std::string getOutput();
	void setOutput(long id , long time) ;
};
#endif

