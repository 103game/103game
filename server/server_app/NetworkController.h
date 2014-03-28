#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <json/json.h>
#include "Common.h"
#include <queue>

#include "PublisherMessage.h"

#define NTWK_STATE_SWITCH_TIME 1 // set state time (seconds)

using namespace std;

class Server; // Server.h уже включает в себя этот h, поэтому делаем так, чтобы не было рекурсии


typedef enum {
	NTWK_LOOP_STATE_RECEIVE, NTWK_LOOP_STATE_SEND
} NTWK_LOOP_STATE;


class NetworkController
{
	public:
		Server *server;
		NetworkController(Server *_server);
		__int64 ticks;

		NTWK_LOOP_STATE networkLoopState;
		void switchState();

		// Every received message from client needs immediate
		// reply from server (thats how server knows where to send message)
		queue<string> receivedMessages; // messages received from clients
		queue<string> immediateReplies; // replies that clients are waiting for (req-reply)
				

		queue<PublisherMessage> messagesToPublish; // replies than need additional computation time (publish)

		
};

#endif