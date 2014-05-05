#include "CompilerOptions.h"

#include "Server.h"

#include "NetworkController.h"
#include "DBController.h"
#include "ServerActions.h"

#include "World.h"
#include "Creatures.h"
#include "SurfaceBlock.h"

#include "UserMapper.h"

#include <boost/thread/thread.hpp>

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;
extern boost::condition_variable receivedMessagesCond, messagesToSendCond;

DBController *sharedDb;


void Server::serverMainLoop(Server *server)
{
	NetworkController *ntw = server->networkController;

	while(true)
	{	

		static clock_t last_asw = 0;

		if((clock()-last_asw)/((double) CLOCKS_PER_SEC) > .5){
			server->serverActions->answerRequests();
		}

		
		//Utils::LOG("SURFACE UPDATE");
		server->world->getSurfaceBlockByCoords(COORDS(rand()%10, rand()%10))
			->setSurfaceType((SURFACE)(rand()%4));
		
		

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
	
	// create world

	this->world = shared_ptr<World>(new World());


	
	for(int i = 0; i < 14; i++){
		for(int j = 0; j < 10; j++){
			world->insertSb(
					shared_ptr<SurfaceBlock>(new SurfaceBlock(COORDS(i, j)))
				);
		}
	}

	Utils::LOG("world size "+to_string(world->sbMap.size()));
	
			
	boost::thread mainLoop(Server::serverMainLoop, this); // start server main loop	
	mainLoop.join();
}


