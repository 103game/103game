
#ifndef SERVER_CLASS_DEF
#define SERVER_CLASS_DEF


class NetworkController;
class DBController;
class ServerActions;


class Server
{
	public:		

	long long ticks; // server loop iterations counter

	NetworkController *networkController;
	DBController *dbController;
	ServerActions *serverActions;

	static void serverMainLoop(Server *server);
	Server();
	
};

#endif