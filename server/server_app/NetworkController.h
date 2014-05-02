#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include "JSONMessage.h"
#include <queue>


#define NTWK_STATE_SWITCH_TIME .1 // set state time (seconds)

using namespace std;

class Server; 

typedef enum {
	NTWK_LOOP_STATE_RECEIVE, NTWK_LOOP_STATE_SEND
} NTWK_LOOP_STATE;


class NetworkController
{
	public:

	Server *server;
	long long ticks;

	NTWK_LOOP_STATE networkLoopState;	
		
	queue<JSONMessage> receivedMessages; 	
	queue<JSONMessage> messagesToSend; 

	void switchState();

	JSONMessage messageReceiver(JSONMessage req);

	static void networkMainLoop(NetworkController *ntw);
	NetworkController(Server *_server);
		
};

#endif