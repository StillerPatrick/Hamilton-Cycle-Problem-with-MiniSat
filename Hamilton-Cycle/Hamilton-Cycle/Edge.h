#pragma once 
#ifndef EDGE_H
#define EDGE_H
#include "Node.h"

class Edge
{
public:
	Edge(Node source, Node desitnation) ; 
	~Edge(void);
	std::string create_edge(Node source , Node destination);
private :
	std::string output ;
	Node source ;
	Node destination;

	
	//Summary of 2 Nodes !
	

};

#endif 
