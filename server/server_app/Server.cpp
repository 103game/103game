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
DBController *sharedDb;


void Server::serverMainLoop(Server *server)
{
	NetworkController *ntw = server->networkController;

	while(true)
	{
		server->serverActions->answerRequests();
		

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


	shared_ptr<World> world = shared_ptr<World>(new World());


	
	SurfaceBlock sb2(COORDS(10, 5));
	SurfaceBlock sb(COORDS(10, 4));
	SurfaceBlock sb3(COORDS(10, 1));

	world->surfaceBlocks.push_back(sb2);
	world->surfaceBlocks.push_back(sb);
	world->surfaceBlocks.push_back(sb3);

	shared_ptr<User> m = shared_ptr<User>(new User("spamgoga@gmail.ru", "123456", "goga"));
	sharedDb->saveObject(*m);


	shared_ptr<User> me = UserMapper::getUserByEmailAndPassword("spamgoga@gmail.ru", "123456");

	if(me != NULL){
		cout << me->toBSON().jsonString() << endl;

		shared_ptr<User> me_returns = shared_ptr<User>(new User());
		me_returns->fromBSON(me->toBSON());
		cout << me_returns->toBSON().jsonString() << endl;




		shared_ptr<Zombie> zomb = shared_ptr<Zombie>(new Zombie());		
		zomb->setLife(45);
		zomb->setBot(false);

		zomb->setUserId(me->getId());

		cout << zomb->toBSON().jsonString() << endl;

		shared_ptr<Zombie> zomb_returns = shared_ptr<Zombie>(new Zombie());
		zomb_returns->fromBSON(zomb->toBSON());
		cout << zomb_returns->toBSON().jsonString() << endl;

		shared_ptr<Survivor> srv = shared_ptr<Survivor>(new Survivor());
		srv->setLife(44);

		world->move(srv, world->getSurfaceBlockByCoords(COORDS(10, 4)));
		world->move(zomb, world->getSurfaceBlockByCoords(COORDS(10, 5)));

		cout << world->toBSON().jsonString(mongo::Strict, 1) << endl;
	}
	


	
	

			
	boost::thread mainLoop(Server::serverMainLoop, this); // start server main loop	
	mainLoop.join();
}


