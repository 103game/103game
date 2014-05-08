
#ifndef SERVER_CLASS_DEF
#define SERVER_CLASS_DEF

#include <memory>

using namespace std;

class NetworkController;
class DBController;
class ServerActions;
class GameActions;


class World;


class Server {
	public:		

	long long ticks; // server loop iterations counter

	NetworkController *networkController;
	DBController *dbController;
	ServerActions *serverActions;
	GameActions *gameActions;

	shared_ptr<World> world;

	static void serverMainLoop(Server *server);
	Server();
	
};

#endif