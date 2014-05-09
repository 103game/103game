#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <queue>
#include "JSONMessage.h"

class Client; 
class JSONMessage;

using namespace std;

typedef enum {
	NTWK_LOOP_STATE_RECEIVE, NTWK_LOOP_STATE_SEND
} NTWK_LOOP_STATE;


class NetworkController
{
	public:

	Client *client;
	long long ticks;
	NTWK_LOOP_STATE networkLoopState;
	

	clock_t last_client_request;
	clock_t last_server_response;


	queue<JSONMessage> receivedMessages; 
	queue<JSONMessage> messagesToSend; 

	NetworkController(Client *_client);
	void switchState();
	void messageReceiver(JSONMessage msg);
	static void networkMainLoop(NetworkController *ntw);

};

#endif