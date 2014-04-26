
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
		Utils::log(string("Unknown message to forward "+msg.getAction()).c_str());
	}
}

void ClientActions::signIn(string email, string password) {

	this->client->app->loginErrorsLabel->setLabel("");

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
		Utils::log(msg.getErrorsString().c_str());
		this->client->app->loginErrorsLabel->setLabel(msg.getErrorsString());
		return;
	}

	Utils::log(msg.getString());

	string session_id = msg.getParams()["session_id"].asString();
			
	this->client->session_id = session_id;
	this->client->is_authorized = true;

	Utils::log(string("Session id waas set to "+session_id).c_str());
}

void ClientActions::signUpCallaback(JSONMessage msg) {
	if(msg.hasErrors()){
		Utils::log(msg.getErrorsString().c_str());
		return;
	}

	Utils::log(msg.getString().c_str());
}
