#ifndef GAMEACTIONS_CLASS_DEF
#define GAMEACTIONS_CLASS_DEF

#include <iostream>

class Server;
class JSONMessage;

using namespace std;


class GameActions {
public:

	Server *server;

	GameActions(Server *_server):server(_server){};	

};


#endif
