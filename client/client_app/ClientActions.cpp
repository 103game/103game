
#include "ClientActions.h"

#include "JSONMessage.h"
#include "Client.h"
#include "Utils.h"

#include "Client.h"

void ClientActions::messageForwarder(JSONMessage msg){
	if(msg.getAction() == "signUpCallback") {
		this->signUpCallaback(msg);
	} else if (msg.getAction() == "signInCallback") {
		this->signInCallback(msg);
	} else {
		Utils::log(string("Unknown message to forward "+msg.getAction()).c_str());
	}
}

void ClientActions::signInCallback(JSONMessage msg) {
	if(msg.hasErrors()){
		Utils::log(msg.getErrorsString().c_str());
		return;
	}

	Utils::log(msg.getString());

	string session_id = msg.getParams()["session_id"].asString();
			
	this->client->session_id = session_id;

	Utils::log(string("Session id waas set to "+session_id).c_str());
}

void ClientActions::signUpCallaback(JSONMessage msg) {
	if(msg.hasErrors()){
		Utils::log(msg.getErrorsString().c_str());
		return;
	}

	Utils::log(msg.getString().c_str());
}
