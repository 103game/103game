#ifndef SERVERACTIONS_CLASS_DEF
#define SERVERACTIONS_CLASS_DEF

#include "JSONMessage.h"

#include "NetworkController.h"

#include "Utils.h"
#include <vector>

#include <boost/shared_ptr.hpp>


#include "User.h"

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;
extern boost::condition_variable receivedMessagesCond, messagesToSendCond;

using namespace std;

class ServerActions {
	public:

		NetworkController *ntw;
		
		ServerActions(NetworkController *_ntw):ntw(_ntw){}

		void messageForwarder(JSONMessage msg){
			if(msg.getAction() == "signup"){
				this->signUp(msg);
			}else if(msg.getAction() == "signin"){
				this->signIn(msg);
			}
		}

		void signIn(JSONMessage msg) {
			vector<string> errors;
			Json::Value params = msg.getParams();	

			string email = params["email"].asString();
			string password = params["password"].asString();

			// TODO validate input strings

			User user = User::getUserByEmailAndPassword(email, password);			
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

				user.saveToDb();
				cout << "User authorized "+user.toJSON()<< endl;

				map<string, string> params;
				params["session_id"] = session_id;

				{
					boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
					ntw->messagesToSend.push(
							JSONMessage::actionmsg("signInCallback", params, msg.getClientId())
							);
				}
			}
		}

		void signUp(JSONMessage msg) {
			vector<string> errors;

			Json::Value params = msg.getParams();			


			string email = params["email"].asString(), 
				   password = params["password"].asString(),
				   name = params["name"].asString();


			// validate input strings TODO: check string length
			if(!Utils::isValidEmailAddress(email)){
				errors.push_back("Invalid email");
			}else{
				// check email
				if(User::emailTaken(email)){
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
				newUser.saveToDb();
				cout << "New user created: "<< newUser.toJSON() << endl; 

				map<string, string> params;
				params["user"] = newUser.toJSON();

				{
					boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
					ntw->messagesToSend.push(
							JSONMessage::actionmsg("signUpCallback", params, msg.getClientId())
							);
				}
			}
		}


};


#endif
