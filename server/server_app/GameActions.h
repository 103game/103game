#ifndef GAMEACTIONS_CLASS_DEF
#define GAMEACTIONS_CLASS_DEF

#include <memory>


class Server;
class WorldObject;
class JSONMessage;

using namespace std;


class GameActions {
public:

	Server *server;

	GameActions(Server *_server):server(_server){};	


	void control(JSONMessage msg);
	void GameActions::updateObjectActions();
};


#endif
