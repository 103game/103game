#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>



#include "NetworkController.h"

#include <exception>

#include <zmq.hpp>
#include <zhelpers.hpp>

#include "JSONMessage.h"





extern boost::mutex threadLocker;


using namespace boost;
using namespace std;



void NetworkController::switchState() {
	if(networkLoopState == NTWK_LOOP_STATE_RECEIVE){
		networkLoopState = NTWK_LOOP_STATE_SEND;
	} else {
		networkLoopState = NTWK_LOOP_STATE_RECEIVE;
	}

	cout << "STATE changed to " << (networkLoopState == NTWK_LOOP_STATE_RECEIVE?"RECEIVE":"SEND") << endl;
}



JSONMessage NetworkController::immediateReply(JSONMessage req) {
	
	cout << "Action " + req.getAction() << endl;
	
	if(req.getAction() == "signup"){

		cout << "Signup request" << endl;

		//threadLocker.lock();
		this->receivedMessages.push(req);
		//threadLocker.unlock();

		return JSONMessage("{\"action\": \"ok\", \"params\": \"p\"}", req.getClientId());
	} else if(req.getAction() == "signin") {
		cout << "Signin message" << endl;
		return JSONMessage("{\"action\": \"ok\", \"params\": \"p\"}", req.getClientId());
	} else {
		cout << "Unknown action in immediate reply " + req.getAction() << endl;
		return JSONMessage("{\"action\": \"ok\", \"params\": \"p\"}", req.getClientId());
	}
}



void networkMainLoop(NetworkController *ntw)
{
	// Setup sockets
	zmq::context_t context(1);	
	// router socket
	zmq::socket_t responder(context, ZMQ_ROUTER);

	// set timeout for 1 sec
	int timeout = 1000; 
	responder.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));

	// start listening
	responder.bind("tcp://*:5556");
	
	// setup timer
	clock_t last_state_started;

	// setup state
	ntw->networkLoopState = NTWK_LOOP_STATE_RECEIVE;
	// save current time
	last_state_started = clock();

	while(true)
	{	
		// Clients always send requests
		// requests of two types: 1) that they are ready to receive message 2) query to server
		// Server answers to 1st-type request if it's in  

		if(ntw->networkLoopState == NTWK_LOOP_STATE_RECEIVE) {
			//cout << "Receiving" << endl;		
			threadLocker.lock();
			
				
				// receive client's address
				string address = s_recv (responder);
				if(address.size()){
					// if we have clients
					// receive message
					string message = s_recv (responder);	

					if(message.size()) {
						cout << "Received "+message+" from " + address << endl;
						
						// create immediate reply with created request
						
						
						JSONMessage rep = ntw->immediateReply(JSONMessage(message, address));
						

						cout << rep.getAction() << rep.getString() << endl;
						// and send it to client
						s_sendmore (responder, rep.getClientId());
						s_send (responder, rep.getString());
						
						
					}
				}							
			
			threadLocker.unlock();
				
		}else if (ntw->networkLoopState == NTWK_LOOP_STATE_SEND) {
			//cout << "Sending" << endl;
			
			if(ntw->cookedMessages.size()) {
				// if we have something to send to clients
				
				
				cout << "cooked messages("<< ntw->cookedMessages.size()<<")"<<endl;
				
				// get next reply from reply queue
				JSONMessage rep = ntw->cookedMessages.front();

				// and send it
				s_sendmore (responder, rep.getClientId());				
				s_send (responder, rep.getString());

				// remove sent reply from queue
				ntw->cookedMessages.pop();
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


NetworkController::NetworkController(Server *_server)
{
	this->server = _server;
	this->ticks = 0;

	
	boost::thread networkThread(networkMainLoop, this);	
}