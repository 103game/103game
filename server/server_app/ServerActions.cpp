#include "CompilerOptions.h"

#include "ServerActions.h"

#include "NetworkController.h"
#include "Server.h"
#include "World.h"

#include "UserMapper.h"
#include "DBController.h"

#include "Creatures.h"

#include "GameActions.h"

#include "BSON.h"
#include "Utils.h"
#include <vector>



#include <boost/thread/thread.hpp>
extern boost::mutex receivedMessagesMutex, messagesToSendMutex;
extern boost::condition_variable receivedMessagesCond, messagesToSendCond;

extern DBController *sharedDb;

ServerActions::ServerActions(NetworkController *_ntw){
	ntw = _ntw;
	server = ntw->server;
}





void ServerActions::answerRequests() {		

	while(ntw->receivedMessages.size()) {		
		JSONMessage req;					
		{		
			boost::lock_guard<boost::mutex> lock(receivedMessagesMutex);			
			req = ntw->receivedMessages.front();								
			ntw->receivedMessages.pop();			
		}

		this->messageForwarder(req);		
		
	}	
}

void ServerActions::messageForwarder(JSONMessage msg){
	if(msg.getAction() == "signup"){
		this->signUp(msg);
	}else if(msg.getAction() == "signin"){
		this->signIn(msg);
	}else if(msg.getAction() == "getWorld"){
		this->getWorld(msg);
	}else if(msg.getAction() == "control"){
		this->server->gameActions->control(msg);
	}else{
		Utils::LOG("unknown requested action: "+msg.getAction());
	}
}




void ServerActions::getWorld(JSONMessage msg){

	auto requester = msg.getUser();
	JSONMessage rpl;

	if(requester){
		Utils::LOG("User "+(requester->getName())+" requested world");
		BSONObj bson = BSON("world" << this->server->world->toBSON() << "userCreature" << requester->getCreature()->toBSON());
		rpl = JSONMessage::actionmsg("getWorldCallback", bson, msg.getClientId());		
	}else{
		rpl = JSONMessage::error("getWorldCallback", "not_auth", msg.getClientId());		
	}	

	{	
		boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
		ntw->messagesToSend.push(rpl);
	}
}


void ServerActions::signOut(shared_ptr<User> usr) {
	usr->setSessionId("");
	sharedDb->saveObject(usr);
}

void ServerActions::signOut(JSONMessage msg) {
	auto requester = msg.getUser();
	if(requester){
		signOut(requester);
	}
}

void ServerActions::signIn(JSONMessage msg) {
	vector<string> errors;
	BSONObj params = msg.getParams();	

	string email = params.getStringField("email");
	string password = params.getStringField("password");

	// TODO validate input strings

	shared_ptr<User> user = UserMapper::getUserByEmailAndPassword(email, password);				
	if(user == NULL){
		errors.push_back("Wrong email or password");
	}

	if(errors.size()){
		boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
		ntw->messagesToSend.push(JSONMessage::errors("signInCallback", errors, msg.getClientId()));
	}else{
		// generate session id
		string session_id = Utils::randomString(16);
		user->setSessionId(session_id);

		sharedDb->saveObject(user);
		cout << "User authorized "+user->toBSON().jsonString()<< endl;

		shared_ptr<Creature> userCreature = user->getCreature();

		if(userCreature == NULL){
			boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
			ntw->messagesToSend.push(JSONMessage::error("signInCallback", "No creature", msg.getClientId()));
			return;
		}
		
		userCreature = static_pointer_cast<Creature>(server->world->respawnObject(userCreature));

		if(userCreature == NULL){
			boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
			ntw->messagesToSend.push(JSONMessage::error("signInCallback", "Cant respawn", msg.getClientId()));
			return;
		}


		Utils::LOG("Creature respawned at "+userCreature->getSurfaceBlock()->getCoords().toString());
		

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

	BSONObj params = msg.getParams();			


	string email = params.getStringField("email"),
			password = params.getStringField("password"),
			name = params.getStringField("name");


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
		shared_ptr<User> usr = shared_ptr<User>(new User(email, password, name));
		// create survivor 
		shared_ptr<Survivor> srv = shared_ptr<Survivor>(new Survivor());
		
		usr->setCreatureId(srv->getId());
		srv->setUserId(usr->getId());
		srv->setBot(false);

		sharedDb->saveObject(usr);
		sharedDb->saveObject(srv);

		cout << "New user created: "<< usr->toBSON().jsonString() << endl; 

		BSONObj params = BSON("user" << usr->toBSON());

		{
			boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
			ntw->messagesToSend.push(
					JSONMessage::actionmsg("signUpCallback", params, msg.getClientId())
					);
		}
	}
}


