#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>


#include <boost/thread/mutex.hpp>


#include "NetworkController.h"
#include <exception>


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
	zmq::socket_t requester(context, ZMQ_REQ);
	int timeout = 2000; // set timeout for 2 secs
	requester.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
	requester.connect("tcp://localhost:5556");

	if(!requester.connected()){
		DBOUT("Not connecteed");
		return;
	}

	
		
	// subscribtion socket
	zmq::socket_t subscriber(context, ZMQ_SUB);

	
	// setup timer
	clock_t last_state_started;

	// setup state
	ntw->networkLoopState = NTWK_LOOP_STATE_SEND;
	last_state_started = clock();

	while(true)
	{	
		
		if(ntw->networkLoopState == NTWK_LOOP_STATE_RECEIVE) {
			DBOUT("Receiving");

			// Then check subscription messages
			if(ntw->subscribed) {
				
			}
				
		}else if (ntw->networkLoopState == NTWK_LOOP_STATE_SEND) {
			DBOUT("Sending");

			// send one of async messages
			if(ntw->messagesToSend.size()){
				string messageString = ntw->messagesToSend.front();
				
				s_send(requester, messageString); // send message

				// wait for fast server reply
				string reply = s_recvf(requester, ZMQ_NOBLOCK);
				if(reply != "ok") {
					cout << "Error on fast reply, server says: " << reply << endl;
				}

				// remove send message from queue
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

	
	boost::thread networkThread(networkMainLoop, this);	
}