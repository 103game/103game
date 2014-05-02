#ifndef GAMEACTIONS_CLASS_DEF
#define GAMEACTIONS_CLASS_DEF

#include <iostream>

class Client;
class JSONMessage;

using namespace std;


class GameActions {
public:

	Client *client;

	GameActions(Client  *_client):client(_client){};	

};


#endif
