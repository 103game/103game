#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <json/json.h>
#include "Common.h"
#include <queue>

#include <zmq.hpp>
#include <zhelpers.hpp>

#include "PublisherMessage.h"

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
		__int64 ticks;

		NTWK_LOOP_STATE networkLoopState;
		void switchState();

		
		queue<string> receivedMessages; // complicated messages 

		// Every received message from client needs immediate
		// reply from server (thats how server knows where to send message)
		string immediateReply(string message);				

		// cooked replies that need additional computation time (publish)
		queue<PublisherMessage> messagesToPublish; 

		
};

#endif