#ifndef CLIENTACTIONS_CLASS_DEF
#define CLIENTACTIONS_CLASS_DEF

#include <iostream>

class Client;
class JSONMessage;

using namespace std;


class ClientActions {
	public:

		Client *client;
		
		ClientActions(Client  *_client):client(_client){};

		void ClientActions::handleServerMessage();

		void messageForwarder(JSONMessage msg);


		void sendControl(string control);

		void getWorld();
		void getWorldCallback(JSONMessage msg);

		void signOut();

		void signIn(string email, string password);
		void signUp(string email, string name, string password, string password_repeat);

		void signInCallback(JSONMessage msg);
		void signUpCallaback(JSONMessage msg);

};


#endif
