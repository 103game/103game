#ifndef SERVERACTIONS_CLASS_DEF
#define SERVERACTIONS_CLASS_DEF

#include "JSONMessage.h"

#include "NetworkController.h"

#include "Utils.h"
#include <vector>

extern boost::mutex threadLocker;

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
			if(!Utils::isValidEmailAddress(email))
				errors.push_back("Invalid email");
			if(password.length() < 5)
				errors.push_back("Password is too weak");
			if(name.length() < 5)
				errors.push_back("Name must contain at least 5 symbols");

			


			if(errors.size()){
				Json::Value resp, params, errors_json;
				resp["action"] = "signUpCallback";
				
				params["error"] = true;
				for(int i = 0; i < errors.size(); i++)
					errors_json.append(errors[i]);
					
				resp["params"] = params;
				
				ntw->messagesToSend.push(JSONMessage::fromObject(resp, msg.getClientId()));

			}else{

			}
		}


};


#endif
