#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>


#include <boost/thread/mutex.hpp>


#include "NetworkController.h"
#include <exception>

#include "Utils.h"

using namespace boost;


mutex threadLocker;


void NetworkController::switchState() {
	if(networkLoopState == NTWK_LOOP_STATE_RECEIVE){
		networkLoopState = NTWK_LOOP_STATE_SEND;
	} else {
		networkLoopState = NTWK_LOOP_STATE_RECEIVE;
	}
}


void subscribeTo(zmq::socket_t socket, string session_id) {
	socket.connect("tcp://localhost:5556");
	socket.setsockopt( ZMQ_SUBSCRIBE, session_id.c_str(), session_id.size());
}




void networkMainLoop(NetworkController *ntw)
{
	// Setup sockets
	zmq::context_t context(1);

	// request socket
	zmq::socket_t requester(context, ZMQ_DEALER);
	int timeout = -1; // set timeout for 2 secs
	requester.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));


	s_set_id (requester);

	string rndStr = Utils::randomString(5);
    requester.setsockopt(ZMQ_IDENTITY, rndStr.c_str(), rndStr.length());

	DBOUT(string("My id is: "+rndStr).c_str())


	requester.connect("tcp://localhost:5556");

	if(!requester.connected()){
		DBOUT("Not connecteed");
		return;
	}

	
	// setup timer
	clock_t last_state_started;

	// setup state
	ntw->networkLoopState = NTWK_LOOP_STATE_SEND;
	last_state_started = clock();

	while(true)
	{	
		
		if(ntw->networkLoopState == NTWK_LOOP_STATE_RECEIVE) {			
			string msg = s_recvf(requester, ZMQ_NOBLOCK);
			if(msg.size()){
				DBOUT(string("Reply from server: "+msg).c_str());
			}			
				
		}else if (ntw->networkLoopState == NTWK_LOOP_STATE_SEND) {					
			s_send (requester, "tic");
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

	
	boost::thread networkThread(networkMainLoop, this);	
}