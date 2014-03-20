
#ifndef SERVER_CLASS_DEF
#define SERVER_CLASS_DEF

#include "NetworkController.h"

class Server
{
	public:		
		__int64 tickCount;
		NetworkController *networkController;

		Server();

};

#endif