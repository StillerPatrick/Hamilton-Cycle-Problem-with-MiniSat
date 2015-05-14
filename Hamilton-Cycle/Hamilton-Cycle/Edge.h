#pragma once
#include "Node.h"

class Edge
{
public:
	Edge(void);
	~Edge(void);
	std::string create_edge(Node source , Node destination);
private :
	int sourceId ;
	int sourceTime;
	int destinationId ;
	int destinationTime ;
	std::string output ;
	//Summary of 2 Nodes !
	

};

