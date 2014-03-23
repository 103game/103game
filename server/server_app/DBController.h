
#ifndef DBCONTROLLER_CLASS_DEF
#define DBCONTROLLER_CLASS_DEF

#include <iostream>
#include <mongo/client/dbclient.h>

using namespace std;
using namespace mongo;

class Server;

class DBController{
	public:
		DBClientConnection *connection;
		Server *server;

		void connect();

		DBController(Server *_server);

};





#endif