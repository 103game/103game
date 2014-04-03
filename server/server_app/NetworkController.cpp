#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>



#include "NetworkController.h"

#include <exception>

#include <zmq.hpp>
#include <zhelpers.hpp>




extern boost::mutex threadLocker;


using namespace boost;



void NetworkController::switchState() {
	if(networkLoopState == NTWK_LOOP_STATE_RECEIVE){
		networkLoopState = NTWK_LOOP_STATE_SEND;
	} else {
		networkLoopState = NTWK_LOOP_STATE_RECEIVE;
	}
}


AddressedReply NetworkController::immediateReply(AddressedRequest req) {

	// PROTOTYPE
	// TODO: JSON format
	
	if(req.messageString == "tic") {
		this->receivedMessages.push(req);
		return AddressedReply("toc", req.senderId);
	}
}



void networkMainLoop(NetworkController *ntw)
{
	// Setup sockets
	zmq::context_t context(1);	
	// router socket
	zmq::socket_t responder(context, ZMQ_ROUTER);
	int timeout = 1000; // set timeout for 2 secs
	responder.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
	responder.setsockopt( ZMQ_IDENTITY, "tcp://127.0.0.1:5556", strlen("tcp://127.0.0.1:5556") );
	responder.bind("tcp://*:5556");
	
	// setup timer
	clock_t last_state_started;

	// setup state
	ntw->networkLoopState = NTWK_LOOP_STATE_RECEIVE;
	last_state_started = clock();

	while(true)
	{	
		// Clients always send requests
		// requests of two types: 1) that they are ready to receive message 2) query to server
		// Server answers to 1st-type request if it's in  

		if(ntw->networkLoopState == NTWK_LOOP_STATE_RECEIVE) {
			cout << "Receiving" << endl;			
			
			try{
				
				// receive client's address
				string address = s_recv (responder);
				if(address.size()){
					// if we have clients
					cout << "Received from " + address << endl;
					string message = s_recv (responder);				
					

					AddressedRequest req(message, address);

					// create immediate reply
					AddressedReply rep = ntw->immediateReply(req);

					// and send it to client
					s_sendmore (responder, rep.receiverId);					
					s_send (responder, rep.messageString);
				}							
			} catch(std::exception &e){
				cout << e.what() << endl;
			}

				
		}else if (ntw->networkLoopState == NTWK_LOOP_STATE_SEND) {
			cout << "Sending" << endl;
			

			//threadLocker.lock();

			if(ntw->cookedMessages.size()) {
				AddressedReply rep = ntw->cookedMessages.front();

				s_sendmore (responder, rep.receiverId);				
				s_send (responder, rep.messageString);

				ntw->cookedMessages.pop();
			}

			//threadLocker.unlock();
			
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

	// this->messagesToPublish.push(PublisherMessage("hello", "to_sub_101"));

	
	boost::thread networkThread(networkMainLoop, this);	
}