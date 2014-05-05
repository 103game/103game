#include "CompilerOptions.h"
#include "Client.h"

#include <boost/thread/thread.hpp>

#include "ClientActions.h"
#include "NetworkController.h"
#include "JSONMessage.h"
#include "World.h"


extern boost::mutex receivedMessagesMutex, messagesToSendMutex;

	
		
Client::Client(ClientApp *_app) {
	this->app = _app;
	this->networkController = new NetworkController(this);
	this->clientActions = new ClientActions(this);
	this->ticks = 0;

	this->last_frame_update = 0;
	this->last_world_update = 0;

	this->is_authorized = false;
	this->session_id = "";

	this->world = shared_ptr<World>(new World());

	
}

void Client::mainLoop(Client *client) {	

	static clock_t last_get_world = 0;

	//while(1){
		client->clientActions->handleServerMessage();	

		if((clock()-last_get_world)/((double) CLOCKS_PER_SEC) > .7){
			client->clientActions->getWorld();
			last_get_world = clock();
		}


		client->ticks++;
	//}	
} 


