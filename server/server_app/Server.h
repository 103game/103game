
#ifndef SERVER_CLASS_DEF
#define SERVER_CLASS_DEF

#include "NetworkController.h"
#include "DBController.h"

#include "ServerActions.h"

class Server
{
	public:		
		__int64 ticks; // server loop iterations counter
		NetworkController *networkController;
		DBController *dbController;
		ServerActions *serverActions;

		Server();

};

#endif