#include "CompilerOptions.h"
#include "ClientActions.h"

#include "JSONMessage.h"
#include "Client.h"
#include "ClientApp.h"

#include "Utils.h"
#include "NetworkController.h"

#include <json/json.h>

#include <boost/thread/thread.hpp>

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;

void ClientActions::messageForwarder(JSONMessage msg){
	if(msg.getAction() == "signUpCallback") {
		this->signUpCallaback(msg);
	} else if (msg.getAction() == "signInCallback") {
		this->signInCallback(msg);
	} else {
		Utils::LOG(string("Unknown message to forward "+msg.getAction()).c_str());
	}
}

void ClientActions::signUp(string email, string name, string password, string password_repeat) {

	this->client->app->signUpErrorsLabel->setLabel("");

	// pre-validate on client TODO:more careful validation
	if(!Utils::isValidEmailAddress(email)){
		this->client->app->signUpErrorsLabel->setLabel("Please enter valid e-mail");
		return;
	}

	if(Utils::trim(name).length() < 4){
		this->client->app->signUpErrorsLabel->setLabel("Too small name");
		return;
	}	

	if(password != password_repeat){
		this->client->app->signUpErrorsLabel->setLabel("Passwords does not match");
		return;
	}

	if(Utils::trim(password).length() < 5){
		this->client->app->signUpErrorsLabel->setLabel("Password must be min 5 chars length");
		return;
	}

	Json::FastWriter writer;

	Json::Value params;
	params["email"] = email;
	params["name"] = email;
	params["password"] = password;

	Json::Value root;
	root["action"] = "signup";
	root["params"] = params;

	JSONMessage msg(writer.write(root));

	boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
	this->client->networkController->messagesToSend.push(msg);

}

void ClientActions::signIn(string email, string password) {

	this->client->app->signInErrorsLabel->setLabel("");

	Json::FastWriter writer;

	Json::Value params;
	params["email"] = email;
	params["password"] = password;

	Json::Value root;
	root["action"] = "signin";
	root["params"] = params;

	JSONMessage msg(writer.write(root));

	boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
	this->client->networkController->messagesToSend.push(msg);

}

void ClientActions::signInCallback(JSONMessage msg) {
	if(msg.hasErrors()){
		Utils::LOG(msg.getErrorsString().c_str());
		this->client->app->signInErrorsLabel->setLabel(msg.getErrorsString());
		return;
	}

	Utils::LOG(msg.getString());

	string session_id = msg.getParams()["session_id"].asString();
			
	this->client->session_id = session_id;
	this->client->is_authorized = true;

	Utils::LOG(string("Session id waas set to "+session_id).c_str());
	this->client->app->setUIState(UI_STATE_SIGNUP);
}

void ClientActions::signUpCallaback(JSONMessage msg) {
	if(msg.hasErrors()){
		this->client->app->signUpErrorsLabel->setLabel(msg.getErrorsString());
		return;
	}

	this->client->app->setUIState(UI_STATE_SIGNIN);

	Utils::LOG(msg.getString());
}
