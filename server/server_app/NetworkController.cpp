#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>

#include "NetworkController.h"



void networkMainLoop(NetworkController *ntw)
{
	while(true)
	{		
		ntw->tickCount++;
	}
}


NetworkController::NetworkController(Server *_server)
{
	this->server = _server;
	this->tickCount = 0;

	
	boost::thread networkThread(networkMainLoop, this);
	
}