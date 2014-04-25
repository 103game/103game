#ifndef CLIENTACTIONS_CLASS_DEF
#define CLIENTACTIONS_CLASS_DEF

class Client;
class JSONMessage;


class ClientActions {
	public:

		Client *client;
		
		ClientActions(Client  *_client):client(_client){};

		void messageForwarder(JSONMessage msg);


		void signInCallback(JSONMessage msg);
		void signUpCallaback(JSONMessage msg);

};


#endif
