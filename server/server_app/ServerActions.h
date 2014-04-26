#ifndef SERVERACTIONS_CLASS_DEF
#define SERVERACTIONS_CLASS_DEF

#include "JSONMessage.h"

class NetworkController;

using namespace std;

class ServerActions {
	public:

		NetworkController *ntw;
		
		ServerActions(NetworkController *_ntw):ntw(_ntw){}

		void messageForwarder(JSONMessage msg);

		void signIn(JSONMessage msg);
		void signUp(JSONMessage msg);


};


#endif
