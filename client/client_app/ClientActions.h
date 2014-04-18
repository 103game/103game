#ifndef CLIENTACTIONS_CLASS_DEF
#define CLIENTACTIONS_CLASS_DEF

#include "JSONMessage.h"
#include "Utils.h"

class Client;

class ClientActions {
	public:

		Client *client;
		
		ClientActions(Client  *_client):client(_client){};

		void messageForwarder(JSONMessage msg){
			if(msg.getAction() == "signUpCallback") {
				this->signUpCallaback(msg);
			}else {
				DBOUT(string("Unknown message to forward "+msg.getAction()).c_str());
			}
		}

		void signUpCallaback(JSONMessage msg) {
			DBOUT("SignUpCallback fired!");
		}


};


#endif
