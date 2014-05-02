#include "CompilerOptions.h"

#include "NetworkController.h"

#include "Server.h"

#include <boost/thread/thread.hpp>



#include <zmq.hpp>
#include <zhelpers.hpp>


#include <exception>

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;




void NetworkController::switchState() {
	if(networkLoopState == NTWK_LOOP_STATE_RECEIVE){
		networkLoopState = NTWK_LOOP_STATE_SEND;
	} else {
		networkLoopState = NTWK_LOOP_STATE_RECEIVE;
	}

	//cout << "STATE changed to " << (networkLoopState == NTWK_LOOP_STATE_RECEIVE?"RECEIVE":"SEND") << endl;
}



JSONMessage NetworkController::messageReceiver(JSONMessage req) {
	
	cout << "Action " + req.getAction() << endl;

	boost::lock_guard<boost::mutex> lock(receivedMessagesMutex);
		
	this->receivedMessages.push(req);
	return JSONMessage::ok(req.getClientId());
		
}



void NetworkController::networkMainLoop(NetworkController *ntw)
{
	// Setup sockets
	zmq::context_t context(1);	
	// router socket
	zmq::socket_t responder(context, ZMQ_ROUTER);

	// set timeout for 1 sec
	int timeout = 1; 
	responder.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
	responder.setsockopt(ZMQ_SNDTIMEO, &timeout, sizeof(timeout));

	// start listening
	responder.bind("tcp://*:5556");
	
	// setup timer
	clock_t last_state_started;

	// setup state
	ntw->networkLoopState = NTWK_LOOP_STATE_RECEIVE;
	// save current time
	last_state_started = clock();

	cout << "Server started" << endl;

	while(true)
	{		

		if(ntw->networkLoopState == NTWK_LOOP_STATE_RECEIVE) {
			//cout << "Receiving" << endl;		
				

			// receive client's address
			string address = s_recv (responder);
			if(address.size()){
				// if we have clients
				// receive message
				string message = s_recv (responder);	

				if(message.size()) {
					cout << "Received "+message+" from " + address << endl;
						
					// create immediate reply with created request					
						
					JSONMessage rep = ntw->messageReceiver(JSONMessage(message, address));					

					cout << rep.getAction() << rep.getString() << endl;
					// and send it to client
					s_sendmore (responder, rep.getClientId());
					s_send (responder, rep.getString());
				}
			}							
			
			
				
		}else if (ntw->networkLoopState == NTWK_LOOP_STATE_SEND) {
			//cout << "Sending" << endl;
			boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
			if(ntw->messagesToSend.size()) {
				// if we have something to send clients

				// get next reply from reply queue
				JSONMessage rep = ntw->messagesToSend.front();
				cout << "Sending message: " /*<< rep.getString()*/ << endl;
				// and send it
				s_sendmore (responder, rep.getClientId());				
				s_send (responder, rep.getString());

				// remove sent reply from queue
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


NetworkController::NetworkController(Server *_server)
{
	this->server = _server;
	this->ticks = 0;

	boost::thread networkThread(networkMainLoop, this);	
}
		
