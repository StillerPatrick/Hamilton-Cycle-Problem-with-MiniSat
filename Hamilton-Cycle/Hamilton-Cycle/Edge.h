#pragma once
#ifndef H_EDGE
#define H_EDGE
#include "Node.h"
class Edge
{
public:
	Edge(Node source, Node destination);
	Node source ;
	Node destination ;
	virtual ~Edge(void);
};
#endif 
