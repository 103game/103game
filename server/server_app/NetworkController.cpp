#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>

#include <zmq.hpp>
#include <zhelpers.hpp>

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



void networkMainLoop(NetworkController *ntw)
{
	// Setup sockets
	zmq::context_t context(1);
	// pub socket	
	zmq::socket_t publisher(context, ZMQ_PUB);
	publisher.bind("tcp://*:5555");
	// res socket
	zmq::socket_t responder(context, ZMQ_REP);
	int timeout = 2000;
	responder.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
	responder.bind("tcp://*:5556");
	
	// setup timer
	clock_t last_state_started;

	// setup state
	ntw->networkLoopState = NTWK_LOOP_STATE_RECEIVE;
	last_state_started = clock();

	while(true)
	{	
		
		if(ntw->networkLoopState == NTWK_LOOP_STATE_RECEIVE) {
			cout << "Receiving" << endl;
			zmq::message_t message;
			

			// receive message
			if(responder.recv(&message, ZMQ_NOBLOCK)) {
				// some error occured
				cout << "Error receiving message: " << zmq_strerror(zmq_errno()) << endl;				
			}
			
			if(message.size()) {
				// if we received something
				// TODO: check message size not too big
				string messageString;
				try{
					// convert incoming data to string					
					messageString = string((char *) message.data());
				}catch(std::exception &e ){
					cout << "Error when converting message data to string: " << e.what() << endl;					
				}				

				// put received message to queue
				ntw->receivedMessages.push(messageString);
			}			
				
		}else{
			cout << "Sending" << endl;

			// first deal with one of immediate replies
			if(ntw->immediateReplies.size()){				
				string messageString = ntw->immediateReplies.front();
				
				zmq::message_t message;
				memcpy(message.data(), messageString.c_str(), messageString.size()+1); // PE
				
				// send message
				if(responder.send(message)) {
					cout << "Error sending message: " << zmq_strerror(zmq_errno()) << endl;
				}

				// remove sent message from queue
				ntw->immediateReplies.pop();
			}

			// now send one of async messages
			if(ntw->messagesToPublish.size()){
				PublisherMessage pubMessage = ntw->messagesToPublish.front();
										
				s_sendmore(publisher, pubMessage.subscriberId); // set subscriber id
				s_send(publisher, pubMessage.messageString); // send message

				// remove send message from queue
				ntw->messagesToPublish.pop();
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

	// this->messagesToPublish.push(PublisherMessage("hello", "to_sub_101"));

	
	boost::thread networkThread(networkMainLoop, this);	
}