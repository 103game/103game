#include "Server.h"

#include "NetworkController.h"
#include "DBController.h"
#include "ServerActions.h"

#include "World.h"
#include "Creatures.h"
#include "SurfaceBlock.h"

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
	
	// place for experiments


	World *world = new World();


	SurfaceBlock sb(COORDS(10, 4));
	SurfaceBlock sb2(COORDS(10, 5));
	SurfaceBlock sb3(COORDS(10, 1));

	world->surfaceBlocks.push_back(sb);
	world->surfaceBlocks.push_back(sb2);
	world->surfaceBlocks.push_back(sb3);


	Zombie *zomb = new Zombie();
	zomb->setLife(55);
	zomb->setBot(false);	

	Zombie *zomb1 = new Zombie();
	zomb1->setLife(25);
	zomb1->setBot(true);

	Survivor *srv = new Survivor();
	srv->setLife(1);

	

	

	
	
	world->move(zomb, world->getSurfaceBlockByCoords(COORDS(10, 5)));
	world->move(zomb1, world->getSurfaceBlockByCoords(COORDS(10, 4)));
	world->move(srv, world->getSurfaceBlockByCoords(COORDS(10, 1)));


	cout << world->toBSON().jsonString(mongo::Strict, 1) << endl;

	
	//cout << obj->toBSON().jsonString() << endl;

	/*

	cout << world->toBSON().jsonString() << endl;*/
	
	

			
	boost::thread mainLoop(Server::serverMainLoop, this); // start server main loop	
	mainLoop.join();
}


