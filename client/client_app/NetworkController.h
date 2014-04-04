#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <json/json.h>
#include "Common.h"
#include <queue>


#include <zmq.hpp>
#include <zhelpers.hpp>

#include "JSONMessage.h"


#define NTWK_STATE_SWITCH_TIME 1 // set state time (seconds)

using namespace std;

class Client; // Server.h уже включает в себя этот h, поэтому делаем так, чтобы не было рекурсии


typedef enum {
	NTWK_LOOP_STATE_RECEIVE, NTWK_LOOP_STATE_SEND
} NTWK_LOOP_STATE;


class NetworkController
{
	public:

		Client *client;
		NetworkController(Client *_client);
		__int64 ticks;


		NTWK_LOOP_STATE networkLoopState;
		void switchState();

		bool subscribed;
		string session_id;


		void messageReceiver(JSONMessage msg);
			

		queue<JSONMessage> receivedMessages; 

		queue<JSONMessage> messagesToSend; 


		
};

#endif