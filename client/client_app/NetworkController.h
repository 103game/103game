#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <json/json.h>
#include <queue>


#include <zmq.hpp>
#include <zhelpers.hpp>

#include "JSONMessage.h"

#include <exception>

#include "Utils.h"


#define NTWK_STATE_SWITCH_TIME 1 

using namespace boost;
using namespace std;

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;
extern boost::condition_variable receivedMessagesCond, messagesToSendCond;


class Client; // Server.h уже включает в себя этот h, поэтому делаем так, чтобы не было рекурсии


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


		
	void switchState() {
		if(networkLoopState == NTWK_LOOP_STATE_RECEIVE){
			networkLoopState = NTWK_LOOP_STATE_SEND;
			DBOUT("STATE changed to SEND");
		} else {
			networkLoopState = NTWK_LOOP_STATE_RECEIVE;
			DBOUT("STATE changed to RECEIVE");
		}

	
	}


	void messageReceiver(JSONMessage msg) {
		//DBOUT(string("MEssage received: "+msg.getString()).c_str());
	
		if(msg.getAction() == "signUpCallback"){	

			{
				boost::lock_guard<boost::mutex> lock(receivedMessagesMutex); // protect receivedMEssages
				this->receivedMessages.push(msg);			
			}

		}else{
			DBOUT(string("Unknown action "+msg.getAction()).c_str());
		}
	}


	static void networkMainLoop(NetworkController *ntw)
	{
	
		// Setup sockets
		zmq::context_t context(1);

		// request socket
		zmq::socket_t requester(context, ZMQ_DEALER);

		// set timeout for infinite
		int timeout = -1; 
		requester.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
	
		// set client identidication code (id)
		string rndStr = Utils::randomString(5);
		requester.setsockopt(ZMQ_IDENTITY, rndStr.c_str(), rndStr.length());
		DBOUT(string("My id is: "+rndStr).c_str());

		// connect to server
		requester.connect("tcp://localhost:5556");

		if(!requester.connected()){
			// if cant reach localhost (not server program)
			DBOUT("Not connecteed");
			return;
		}

	
		// setup timer
		clock_t last_state_started;

		// setup state
		ntw->networkLoopState = NTWK_LOOP_STATE_SEND;
		// save current time
		last_state_started = clock();

		while(true)
		{		

			if(ntw->networkLoopState == NTWK_LOOP_STATE_RECEIVE) {	
				// receive message from server if it exists				
				string jsonStr = s_recvf(requester, ZMQ_NOBLOCK);	
						
				if(jsonStr.length()){			
					JSONMessage msg(jsonStr);				
					ntw->messageReceiver(msg);
				}		
			
			}else if (ntw->networkLoopState == NTWK_LOOP_STATE_SEND) {			
				// send message to server		
				static int c = 0;
				if(c<1){
				JSONMessage msg("{\"action\": \"signup\", \"params\": {\"email\":\"spamgoga@gmail.com\", \"password\":\"12356\", \"name\":\"george\"}}");
				s_sendf(requester, msg.getString(), ZMQ_NOBLOCK);
				}
				c++;

			}

			// check if switch needed
			double state_seconds = (clock() - last_state_started)/(double) CLOCKS_PER_SEC;
			if(state_seconds > NTWK_STATE_SWITCH_TIME) {
				ntw->switchState();
				last_state_started = clock(); 
			}

			// update ticks counter
			ntw->ticks++;
		}
	}


	NetworkController(Client  *_client)
	{
		this->client = _client;
		this->ticks = 0;
		this->subscribed = false;

		// this->messagesToPublish.push(PublisherMessage("hello", "to_sub_101"));

	
		boost::thread networkThread(NetworkController::networkMainLoop, this);	
	}

		
};

#endif