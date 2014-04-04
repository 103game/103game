#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <json/json.h>
#include "Common.h"
#include <queue>


#include "JSONMessage.h"




#define NTWK_STATE_SWITCH_TIME 1 // set state time (seconds)

using namespace std;

class Server; // Server.h ��� �������� � ���� ���� h, ������� ������ ���, ����� �� ���� ��������


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

		
		queue<JSONMessage> receivedMessages; // complicated messages 

		// Every received message from client needs immediate
		// reply from server 
<<<<<<< HEAD
		AddressedReply immediateReply(AddressedRequest req);				
=======
		JSONMessage NetworkController::immediateReply(JSONMessage req);				
>>>>>>> 5b8fa32fe4c9f433cb0dc0980132d8107b1825de

		// cooked replies that need additional computation time
		queue<JSONMessage> cookedMessages; 

		
};

#endif