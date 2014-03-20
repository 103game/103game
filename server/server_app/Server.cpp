#include <iostream>
#include <boost/thread/thread.hpp>


#include "Server.h"


void serverMainLoop(Server *server)
{
	while(true)
	{		
		server->tickCount++;
	}
}

Server::Server()
{
	this->networkController = new NetworkController(this);
	this->tickCount = 0;



	boost::thread mainLoop(serverMainLoop, this); // start server main loop	
	mainLoop.join();
}

