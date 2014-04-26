#include "Server.h"

#include "NetworkController.h"
#include "DBController.h"
#include "ServerActions.h"

#include <boost/thread/thread.hpp>

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;
DBController *sharedDb;


void Server::serverMainLoop(Server *server)
{
	NetworkController *ntw = server->networkController;

	while(true)
	{	

		{
			boost::lock_guard<boost::mutex> lock(receivedMessagesMutex);

			if(ntw->receivedMessages.size()) {

				JSONMessage req = ntw->receivedMessages.front();								
				server->serverActions->messageForwarder(req);
				ntw->receivedMessages.pop();		
			}
		}
		

		server->ticks++;
	}
}

Server::Server()
{	

	sharedDb = this->dbController = new DBController(this);
	if(!this->dbController->connect()){
		cout << "Can't connect to db" << endl;
		return;
	}


	this->networkController = new NetworkController(this);
	this->serverActions = new ServerActions(this->networkController);
	this->ticks = 0;

			
	boost::thread mainLoop(Server::serverMainLoop, this); // start server main loop	
	mainLoop.join();
}


