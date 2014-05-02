#ifndef CLIENT_CLASS_DEF
#define CLIENT_CLASS_DEF

#include <string>
#include <memory>

class ClientApp;
class ClientActions;
class NetworkController;

class World;

using namespace std;

class Client {
	public:		
		NetworkController *networkController;
		ClientApp *app;
		ClientActions *clientActions;
		long long ticks;

		bool is_authorized;
		string session_id;

		shared_ptr<World> world;

		Client(ClientApp *_app);
		void mainLoop();
};

#endif
