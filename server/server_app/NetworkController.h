#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <json/json.h>
#include "Common.h"
#include <queue>

#define NTWK_STATE_SWITCH_TIME 3 // set state time (seconds)

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

		queue<string> receivedMessages;
		queue<string> messagesToSend;

		
};

#endif