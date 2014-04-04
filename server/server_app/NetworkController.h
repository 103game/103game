#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <json/json.h>
#include "Common.h"
#include <queue>



#include "AddressedReply.h"
#include "AddressedRequest.h"


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
		long long ticks;


		NTWK_LOOP_STATE networkLoopState;
		void switchState();

		
		queue<AddressedRequest> receivedMessages; // complicated messages 

		// Every received message from client needs immediate
		// reply from server 
		AddressedReply immediateReply(AddressedRequest req);				

		// cooked replies that need additional computation time
		queue<AddressedReply> cookedMessages; 

		
};

#endif