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
			}
		}

		void signUp(JSONMessage msg) {
			vector<string> errors;

			Json::Value params = msg.getParams();			


			string email = params["email"].asString(), 
				   password = params["password"].asString(),
				   name = params["name"].asString();


			// validate input strings
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
				auto usr = boost::shared_ptr<User>(new User(email, password, name));
				usr->saveToDb();
				cout << "New user created with id: " << usr->id << endl; 

				/*map<string, string> params;
				params["user"] = newUser.toJSON();

				{
					boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
					ntw->messagesToSend.push(
							JSONMessage::actionmsg("signUpCallback", params, msg.getClientId())
							);
				}*/
			}
		}


};


#endif
