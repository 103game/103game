#include "CompilerOptions.h"
#include "Client.h"

#include <boost/thread/thread.hpp>

#include "ClientActions.h"
#include "NetworkController.h"
#include "JSONMessage.h"


extern boost::mutex receivedMessagesMutex, messagesToSendMutex;

	
		
Client::Client(ClientApp *_app) {
	this->app = _app;
	this->networkController = new NetworkController(this);
	this->clientActions = new ClientActions(this);
	this->ticks = 0;

	this->is_authorized = false;
	this->session_id = "";
}

void Client::mainLoop() {
			
	boost::unique_lock<boost::mutex> lock(receivedMessagesMutex);
	//receivedMessagesCond.wait(lock);
			
	if(this->networkController->receivedMessages.size()){
		JSONMessage req = this->networkController->receivedMessages.front();							
		this->clientActions->messageForwarder(req);

		this->networkController->receivedMessages.pop();
				
	}else{
		//DBOUT("NO INCOMING MESSAGES");
	}

	
			

	this->ticks++;
} 


