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

string Edge::create_edge(Node source , Node destination){

	stringstream buffer ;
	buffer << "-" << source.setOutput << " " << destination.getOutput ;
	this->output = buffer.str() ;
	return buffer.str();


}