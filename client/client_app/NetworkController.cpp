#include "CompilerOptions.h"
#include "NetworkController.h"

#include "Client.h"

#include <zmq.hpp>
#include <zhelpers.hpp>

#include "Utils.h"

#include <exception>

#include <boost/thread/thread.hpp>

#define NTWK_STATE_SWITCH_TIME .5

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;


void NetworkController::switchState() {
	if(networkLoopState == NTWK_LOOP_STATE_RECEIVE){
		networkLoopState = NTWK_LOOP_STATE_SEND;
		//Utils::log("STATE changed to SEND");
	} else {
		networkLoopState = NTWK_LOOP_STATE_RECEIVE;
		//Utils::LOG("STATE changed to RECEIVE");
	}	
}


void NetworkController::messageReceiver(JSONMessage msg) {
	//DBOUT(string("MEssage received: "+msg.getString()).c_str());
	
	boost::lock_guard<boost::mutex> lock(receivedMessagesMutex); // protect receivedMEssages
	this->receivedMessages.push(msg);			
		
}


void NetworkController::networkMainLoop(NetworkController *ntw)
{
	
	// Setup sockets
	zmq::context_t context(1);

	// request socket
	zmq::socket_t requester(context, ZMQ_DEALER);

	// set timeout for infinite
	int timeout = 0; 
	requester.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
	requester.setsockopt(ZMQ_SNDTIMEO, &timeout, sizeof(timeout));
	
	// set client identidication code (id)
	string rndStr = Utils::randomString(16);
	requester.setsockopt(ZMQ_IDENTITY, rndStr.c_str(), rndStr.length());
	Utils::LOG(string("My id is: "+rndStr).c_str());

	// connect to server
	requester.connect("tcp://localhost:5556");

	if(!requester.connected()){
		// if cant reach localhost (not server program)
		Utils::LOG("Not connecteed");
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
			string jsonStr = s_recv(requester);				
			if(jsonStr.length()){		
				JSONMessage msg(jsonStr);				
				Utils::LOG("Received "+msg.toString());
				ntw->messageReceiver(msg);
			}		
			
		}else if (ntw->networkLoopState == NTWK_LOOP_STATE_SEND) {			
			// send message to server		
			boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
			if(ntw->messagesToSend.size()) {				
				JSONMessage msg = ntw->messagesToSend.front();
				Utils::LOG("Sending "+msg.toString());
				s_send(requester, msg.toString());
				ntw->messagesToSend.pop();				
			}
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


NetworkController::NetworkController(Client  *_client)
{
	this->client = _client;
	this->ticks = 0;
	this->subscribed = false;

	// this->messagesToPublish.push(PublisherMessage("hello", "to_sub_101"));

	
	boost::thread networkThread(NetworkController::networkMainLoop, this);	
}