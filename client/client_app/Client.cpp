#include "CompilerOptions.h"
#include "Client.h"

#include <boost/thread/thread.hpp>

#include "ClientActions.h"
#include "GameActions.h"
#include "NetworkController.h"
#include "JSONMessage.h"
#include "World.h"

#include "ClientApp.h"

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;

	
		
Client::Client(ClientApp *_app) {
	this->app = _app;
	this->networkController = new NetworkController(this);
	this->clientActions = new ClientActions(this);
	this->gameActions = new GameActions(this);
	this->ticks = 0;

	this->last_frame_update = 0;
	this->last_world_update = 0;

	this->is_authorized = false;
	this->session_id = "";
	this->userCreature = NULL;

	this->world = shared_ptr<World>(new World());

	
}

void Client::mainLoop(Client *client) {	


	while(true){
		clock_t handle_start = clock();
		client->clientActions->handleServerMessage();	
		//Utils::LOG("HANDLE_MSG_TIME: "+to_string((clock() - handle_start)/(double) CLOCKS_PER_SEC));		

		static clock_t last_get_world = 0;

		if(client->is_authorized){
			if((clock()-last_get_world)/((double) CLOCKS_PER_SEC) > .3){
				client->clientActions->getWorld();
				last_get_world = clock();
			}
		}

		//Utils::LOG("TICK_TIME: "+to_string((clock() - handle_start)/(double) CLOCKS_PER_SEC));

		client->ticks++;
	}
	
	
} 


