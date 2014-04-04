
#ifndef SERVER_CLASS_DEF
#define SERVER_CLASS_DEF

#include "NetworkController.h"
#include "DBController.h"

#include "ServerActions.h"

class Server
{
	public:		
<<<<<<< HEAD
		long long ticks; // количество итераций главного цикла
=======
		__int64 ticks; // server loop iterations counter
>>>>>>> 5b8fa32fe4c9f433cb0dc0980132d8107b1825de
		NetworkController *networkController;
		DBController *dbController;
		ServerActions *serverActions;

		Server();

};

#endif