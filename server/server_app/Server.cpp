#include <iostream>
#include <boost/thread/thread.hpp>


#include "Server.h"

#include <boost/thread/mutex.hpp>

extern boost::mutex threadLocker;


void serverMainLoop(Server *server)
{
	while(true)
	{		
		// Cook responses	
		
		NetworkController *ntw = server->networkController;

		if(ntw->receivedMessages.size()) {

			JSONMessage req = ntw->receivedMessages.front();			
			cout << "get from queue action " << req.getAction() << endl;
			server->serverActions->messageForwarder(req);

			ntw->receivedMessages.pop();		
		}
		

		server->ticks++;
	}
}

Server::Server()
{	
	this->networkController = new NetworkController(this);

	this->dbController = new DBController(this);
	this->dbController->connect(); // connect

	this->serverActions = new ServerActions(this->networkController);

	this->ticks = 0;



	boost::thread mainLoop(serverMainLoop, this); // start server main loop	
	mainLoop.join();
}

