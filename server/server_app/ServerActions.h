#ifndef SERVERACTIONS_CLASS_DEF
#define SERVERACTIONS_CLASS_DEF

#include "JSONMessage.h"

#include "Server.h"

class ServerActions {
	public:

		Server *server;
		
		ServerActions(Server *_srv):server(_srv){};

		void signUp(JSONMessage) {

		}


};


#endif
