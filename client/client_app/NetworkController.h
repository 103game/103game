#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <queue>
#include "JSONMessage.h"

#define NTWK_STATE_SWITCH_TIME 1 

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
	bool subscribed;
	string session_id;

	queue<JSONMessage> receivedMessages; 
	queue<JSONMessage> messagesToSend; 

	NetworkController(Client *_client);
	void switchState();
	void messageReceiver(JSONMessage msg);
	static void networkMainLoop(NetworkController *ntw);

};

#endif