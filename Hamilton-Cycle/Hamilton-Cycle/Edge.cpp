#include "stdafx.h"
#include "Edge.h"
#include <sstream>


using namespace std ;


Edge::Edge(Node source , Node destination)
{
	this->source = source ;
	this->destination = destination;
}


Edge::~Edge(void)
{
}




