#ifndef CLIENT_CLASS_DEF
#define CLIENT_CLASS_DEF

#include <string>
#include <memory>
#include <time.h>

#include "Creatures.h"

class ClientApp;
class ClientActions;
class NetworkController;
class GameActions;


class World;

using namespace std;

class Client {
	public:		
		NetworkController *networkController;
		ClientApp *app;
		ClientActions *clientActions;
		GameActions *gameActions;
		long long ticks;

		bool is_authorized;
		string session_id;
		shared_ptr<Creature> userCreature;

		shared_ptr<World> world;

		clock_t last_world_update, last_frame_update;
		double wps, fps;

		Client(ClientApp *_app);
		static void mainLoop(Client *client);
};

#endif
