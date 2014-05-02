#ifndef SERVERACTIONS_CLASS_DEF
#define SERVERACTIONS_CLASS_DEF

#include "JSONMessage.h"

class NetworkController;
class Server;

using namespace std;

class ServerActions {
	public:

		NetworkController *ntw;
		Server *server;

		ServerActions(NetworkController *_ntw);
		
		

		void ServerActions::answerRequest();
		void messageForwarder(JSONMessage msg);

		void signIn(JSONMessage msg);
		void signUp(JSONMessage msg);

		void getWorld(JSONMessage msg);


};


#endif
