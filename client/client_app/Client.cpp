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

void Client::mainLoop() {	
	this->clientActions->handleServerMessage();	

	
	clientActions->getWorld();
	
	

	this->ticks++;
} 


