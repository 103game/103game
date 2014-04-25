#ifndef CLIENT_CLASS_DEF
#define CLIENT_CLASS_DEF

#include <string>

class ClientApp;
class ClientActions;
class NetworkController;

using namespace std;

class Client {
	public:		
		NetworkController *networkController;
		ClientApp *app;
		ClientActions *clientActions;
		long long ticks;
		string session_id;

		Client(ClientApp *_app);
		void mainLoop();
};

#endif
