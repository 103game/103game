
#ifndef SERVER_CLASS_DEF
#define SERVER_CLASS_DEF


#include "DBController.h"
#include "NetworkController.h"


#include "ServerActions.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "User.h"

extern boost::mutex threadLocker;

DBController *sharedDb;

class Server
{
	public:		

	long long ticks; // server loop iterations counter

	NetworkController *networkController;
	DBController *dbController;
	ServerActions *serverActions;

	static void serverMainLoop(Server *server)
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

	Server()
	{	

		sharedDb = this->dbController = new DBController(this);
		if(!this->dbController->connect()){
			cout << "Can't connect to db" << endl;
			//return;
		}


		this->networkController = new NetworkController(this);

		this->serverActions = new ServerActions(this->networkController);

		this->ticks = 0;

		User usr("spamgoga@gmail.com", "qwerty", "george");
		usr.toJSON();

		boost::thread mainLoop(serverMainLoop, this); // start server main loop	
		mainLoop.join();
	}
};

#endif