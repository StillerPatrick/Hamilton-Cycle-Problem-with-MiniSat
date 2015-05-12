#include "stdafx.h"
#include "Node.h"

using namespace std ;

Node::Node(int  id , int time )
{
	
	this->id = id  ;
	this->time = time;
}


Node::~Node(void)
{
}

int  Node::getID()
{
	return this->id ;
}

int Node::getTime()
{
	return this->time ;
}

std::string Node::getOutput()
{
	return this->output ;
}