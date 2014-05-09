#include "CompilerOptions.h"

#include "Server.h"

#include "NetworkController.h"
#include "DBController.h"
#include "ServerActions.h"
#include "GameActions.h"

#include "World.h"
#include "Creatures.h"
#include "SurfaceBlock.h"

#include "UserMapper.h"


#include <boost/thread/thread.hpp>

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;
extern boost::condition_variable receivedMessagesCond, messagesToSendCond;

DBController *sharedDb;

shared_ptr<Zombie> zmb;

void Server::serverMainLoop(Server *server)
{
	NetworkController *ntw = server->networkController;
	auto world = server->world;

	while(true)
	{	

		static clock_t zmb_move = 0;
		static bool moving_right = true;

		if((clock()-zmb_move)/((double) CLOCKS_PER_SEC) > 1){

			if(moving_right)
				if(!world->moveRight(zmb))
					moving_right = false;
				else
					;
			else
				if(!world->moveLeft(zmb))
					moving_right = true;
				
			zmb_move = clock();
		}

		server->serverActions->answerRequests();

		
		//Utils::LOG("SURFACE UPDATE");
		/*server->world->getSurfaceBlockByCoords(COORDS(rand()%10, rand()%10))
			->setSurfaceType((SURFACE)(rand()%4));*/
		
		

		server->ticks++;
	}
}

Server::Server()
{		

	sharedDb = this->dbController = new DBController(this);
	if(!this->dbController->connect()){
		Utils::LOG("Can't connect to db");
		cin.get();
		return;
	}


	this->networkController = new NetworkController(this);
	this->serverActions = new ServerActions(this->networkController);
	this->gameActions = new GameActions(this);
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

	zmb = shared_ptr<Zombie>(new Zombie());
	world->move(zmb, world->getSurfaceBlockByCoords(COORDS(1, 1)));

	Utils::LOG("world size "+to_string(world->sbMap.size()));



	
	boost::thread mainLoop(Server::serverMainLoop, this); // start server main loop	
	mainLoop.join();
}


