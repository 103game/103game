#include "CompilerOptions.h"
#include "ClientActions.h"

#include "JSONMessage.h"
#include "Client.h"
#include "ClientApp.h"

#include "Utils.h"
#include "NetworkController.h"

#include <boost/thread/thread.hpp>

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;
extern boost::mutex worldMutex;

void ClientActions::handleServerMessage(){	

	while(client->networkController->receivedMessages.size()){
		JSONMessage req;
		{
			boost::lock_guard<boost::mutex> lock(receivedMessagesMutex);
			req = client->networkController->receivedMessages.front();								
			client->networkController->receivedMessages.pop();
		}		
		
		client->clientActions->messageForwarder(req);
	}
}

void ClientActions::messageForwarder(JSONMessage msg){
	if(msg.getAction() == "signUpCallback") {
		this->signUpCallaback(msg);
	} else if (msg.getAction() == "signInCallback") {
		this->signInCallback(msg);
	}else if(msg.getAction() == "getWorldCallback"){
		getWorldCallback(msg);
	} else {
		Utils::LOG(string("Unknown message to forward "+msg.getAction()).c_str());
	}
}


void ClientActions::getWorld() {
	BSONObj bson = BSON(
		"action" << "getWorld"
		<< "params" << BSONObj()
		);

	JSONMessage msg(bson, client->session_id);

	boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
	this->client->networkController->messagesToSend.push(msg);	
}

void ClientActions::getWorldCallback(JSONMessage msg) {
	
	if(msg.hasErrors()){
		client->is_authorized = false;
		client->session_id = "";
		client->app->alertView->setAlert("Connection lost", "Disconnected from server");
		this->client->app->setUIState(UI_STATE_SIGNIN);
		return;
	}

	// get user creature
	client->userCreature = shared_ptr<Creature>(new Creature());
	client->userCreature->fromBSON(msg.getParams().getField("userCreature").Obj());

	// get world
	shared_ptr<World> w = shared_ptr<World>(new World());
	w->fromBSON(msg.getParams().getField("world").Obj());

	Utils::LOG("world received. Size: "+to_string(w->sbMap.size()));

	boost::lock_guard<boost::mutex> lock(worldMutex);
	this->client->world = w;

	this->client->wps = (clock() - this->client->last_world_update)/(double) CLOCKS_PER_SEC;
	this->client->last_world_update = clock();
}


void ClientActions::signUp(string email, string name, string password, string password_repeat) {

	this->client->app->signUpView->setErrorLabel("");

	// pre-validate on client TODO:more careful validation
	if(!Utils::isValidEmailAddress(email)){
		this->client->app->signUpView->setErrorLabel("Please enter valid e-mail");
		return;
	}

	if(Utils::trim(name).length() < 4){
		this->client->app->signUpView->setErrorLabel("Too small name");
		return;
	}	

	if(password != password_repeat){
		this->client->app->signUpView->setErrorLabel("Passwords does not match");
		return;
	}

	if(Utils::trim(password).length() < 5){
		this->client->app->signUpView->setErrorLabel("Password must be min 5 chars length");
		return;
	}

	BSONObj bson = BSON(
			"action" << "signup"
			<< "params" << BSON(
				"email" << email
				<< "name" << name
				<< "password" << password
			)
		);

	JSONMessage msg(bson);

	boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
	this->client->networkController->messagesToSend.push(msg);

}

void ClientActions::signIn(string email, string password) {

	this->client->app->signInView->setErrorLabel("");

	BSONObj bson = BSON(
		"action" << "signin"
		<< "params" << BSON(			
			"email" << email		
			<< "password" << password
		)
		);

	JSONMessage msg(bson);

	boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
	this->client->networkController->messagesToSend.push(msg);

}

void ClientActions::signInCallback(JSONMessage msg) {
	if(msg.hasErrors()){
		Utils::LOG(msg.getErrorsString().c_str());
		this->client->app->signInView->setErrorLabel(msg.getErrorsString());
		return;
	}

	Utils::LOG(msg.toString());

	string session_id = msg.getParams().getStringField("session_id");
			
	this->client->session_id = session_id;
	this->client->is_authorized = true;

	Utils::LOG(string("Session id waas set to "+session_id).c_str());
	this->client->app->setUIState(UI_STATE_GAME);		
	//this->client->app->setUIState(UI_STATE_SIGNUP);
}

void ClientActions::signUpCallaback(JSONMessage msg) {
	Utils::LOG("signup callback has errors: "+to_string(msg.hasErrors()));
	if(msg.hasErrors()){
		this->client->app->signUpView->setErrorLabel(msg.getErrorsString());
		return;
	}


	this->client->app->setUIState(UI_STATE_SIGNIN);
	Utils::LOG(msg.toString());
}
