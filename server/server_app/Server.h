
#ifndef SERVER_CLASS_DEF
#define SERVER_CLASS_DEF

#include "NetworkController.h"
#include "DBController.h"


class Server
{
	public:		
		long long ticks; // количество итераций главного цикла
		NetworkController *networkController;
		DBController *dbController;

		Server();

};

#endif