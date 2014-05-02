#include "CompilerOptions.h"
#include "ClientActions.h"

#include "JSONMessage.h"
#include "Client.h"
#include "ClientApp.h"

#include "Utils.h"
#include "NetworkController.h"

#include <boost/thread/thread.hpp>

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;

void ClientActions::handleServerMessage(){
	boost::unique_lock<boost::mutex> lock(receivedMessagesMutex);

	if(client->networkController->receivedMessages.size()){
		JSONMessage req = client->networkController->receivedMessages.front();							
		client->clientActions->messageForwarder(req);

		client->networkController->receivedMessages.pop();

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

	JSONMessage msg(bson.jsonString());

	boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
	this->client->networkController->messagesToSend.push(msg);
}

void ClientActions::getWorldCallback(JSONMessage msg) {
	
	shared_ptr<World> w = shared_ptr<World>(new World());
	w->fromBSON(msg.getParams());
	Utils::LOG("world received. Size: "+to_string(w->sbMap.size()));
	this->client->world = w;
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

	JSONMessage msg(bson.jsonString());

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

	JSONMessage msg(bson.jsonString());

	boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
	this->client->networkController->messagesToSend.push(msg);

}

void ClientActions::signInCallback(JSONMessage msg) {
	if(msg.hasErrors()){
		Utils::LOG(msg.getErrorsString().c_str());
		this->client->app->signInView->setErrorLabel(msg.getErrorsString());
		return;
	}

	Utils::LOG(msg.getString());

	string session_id = msg.getParams().getStringField("session_id");
			
	this->client->session_id = session_id;
	this->client->is_authorized = true;

	Utils::LOG(string("Session id waas set to "+session_id).c_str());
	//this->client->app->setUIState(UI_STATE_SIGNUP);
}

void ClientActions::signUpCallaback(JSONMessage msg) {
	Utils::LOG("signup callback has errors: "+to_string(msg.hasErrors()));
	if(msg.hasErrors()){
		this->client->app->signUpView->setErrorLabel(msg.getErrorsString());
		return;
	}
	
	//this->client->app->setUIState(UI_STATE_SIGNIN);
		

	Utils::LOG(msg.getString());
}
