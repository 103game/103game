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
		
		

		void answerRequests();
		void messageForwarder(JSONMessage msg);

		void signIn(JSONMessage msg);
		void signUp(JSONMessage msg);
		void signOut(JSONMessage msg);
		void signOut(shared_ptr<User> usr);

		void getWorld(JSONMessage msg);


};


#endif
