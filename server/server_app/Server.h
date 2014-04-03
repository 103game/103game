
#ifndef SERVER_CLASS_DEF
#define SERVER_CLASS_DEF

#include "NetworkController.h"
#include "DBController.h"


class Server
{
	public:		
		__int64 ticks; // ���������� �������� �������� �����
		NetworkController *networkController;
		DBController *dbController;

		Server();

};

#endif