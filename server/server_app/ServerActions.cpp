#include "ServerActions.h"

#include "NetworkController.h"


#include "Utils.h"
#include <vector>
#include "UserMapper.h"

#include "DBController.h"



#include <boost/thread/thread.hpp>
extern boost::mutex receivedMessagesMutex, messagesToSendMutex;

extern DBController *sharedDb;

using namespace bson;

void ServerActions::messageForwarder(JSONMessage msg){
	if(msg.getAction() == "signup"){
		this->signUp(msg);
	}else if(msg.getAction() == "signin"){
		this->signIn(msg);
	}
}

void ServerActions::signIn(JSONMessage msg) {
	vector<string> errors;
	Json::Value params = msg.getParams();	

	string email = params["email"].asString();
	string password = params["password"].asString();

	// TODO validate input strings

	User user = UserMapper::getUserByEmailAndPassword(email, password);			
	if(!user.isInDb()){
		errors.push_back("Wrong email or password");
	}

	if(errors.size()){
		boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
		ntw->messagesToSend.push(JSONMessage::errors("signInCallback", errors, msg.getClientId()));
	}else{
		// generate session id
		string session_id = Utils::randomString(16);
		user.session_id = session_id;

		sharedDb->saveObject(user);
		cout << "User authorized "+user.toBSON().jsonString()<< endl;

		BSONObj params = BSON("session_id" << session_id);
		

		{
			boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
			ntw->messagesToSend.push(
					JSONMessage::actionmsg("signInCallback", params, msg.getClientId())
					);
		}
	}
}

void ServerActions::signUp(JSONMessage msg) {
	vector<string> errors;

	Json::Value params = msg.getParams();			


	string email = params["email"].asString(), 
			password = params["password"].asString(),
			name = params["name"].asString();


	// validate input strings TODO:more careful validation
	if(!Utils::isValidEmailAddress(email)){
		errors.push_back("Invalid email");
	}else{
		// check email
		if(UserMapper::emailTaken(email)){
			errors.push_back("Email "+email+" is already in use");
		}
	}

	if(password.length() < 5)
		errors.push_back("Password is too weak");
	if(name.length() < 5)
		errors.push_back("Name must contain at least 5 symbols");

			


	if(errors.size()){	
		boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
		ntw->messagesToSend.push(JSONMessage::errors("signUpCallback", errors, msg.getClientId()));
	}else{
		User newUser(email, password, name);
		sharedDb->saveObject(newUser);
		cout << "New user created: "<< newUser.toBSON().jsonString() << endl; 

		BSONObj params = BSON("user" << newUser.toBSON());

		{
			boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
			ntw->messagesToSend.push(
					JSONMessage::actionmsg("signUpCallback", params, msg.getClientId())
					);
		}
	}
}


