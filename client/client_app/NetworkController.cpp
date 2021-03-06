#include "CompilerOptions.h"
#include "NetworkController.h"

#include "Client.h"

#include <zmq.hpp>
#include <zhelpers.hpp>

#include "Utils.h"

#include <exception>

#include <boost/thread/thread.hpp>

#define NTWK_STATE_SWITCH_TIME .05

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
	/*int timeout = 0; 
	requester.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
	requester.setsockopt(ZMQ_SNDTIMEO, &timeout, sizeof(timeout));*/

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
			clock_t receive_start = clock();
			string jsonStr = s_recvf(requester, ZMQ_NOBLOCK);				
			if(jsonStr.length()){						
				clock_t json_parse_start = clock();
				JSONMessage msg(jsonStr);								
				//msg.parse();
				//Utils::LOG("Received "+msg.toString());
				ntw->messageReceiver(msg);
				Utils::LOG("QUEUE_RCV_SIZE: "+to_string(ntw->receivedMessages.size()));
				Utils::LOG("SINGLE_RCV_TIME: "+to_string((clock() - receive_start)/(double) CLOCKS_PER_SEC));				
				ntw->last_server_response = clock();
			}		

		}else if (ntw->networkLoopState == NTWK_LOOP_STATE_SEND) {			
			// send message to server		
			boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
			if(ntw->messagesToSend.size()) {		
				Utils::LOG("QUEUE_SND_SIZE: "+to_string(ntw->messagesToSend.size()));
				JSONMessage msg = ntw->messagesToSend.front();
				Utils::LOG("Sending "+msg.toString());
				s_send(requester, msg.toString());
				ntw->messagesToSend.pop();			
				ntw->last_client_request = clock();
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

	last_server_response = last_client_request = 0;


	// this->messagesToPublish.push(PublisherMessage("hello", "to_sub_101"));


	boost::thread networkThread(NetworkController::networkMainLoop, this);	
}