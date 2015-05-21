#include "stdafx.h"
#include "Node.h"
#include <sstream>

using namespace std ;

Node::Node(int  id , int time )
{
	
	this->id = id  ;
	this->time = time;
	this->setOutput(id,time) ;
}

Node::Node(void) {


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

void Node::setOutput(long id , long time)
{
	stringstream buffer ;
	buffer << id << time  ;
	this->output = buffer.str();
}