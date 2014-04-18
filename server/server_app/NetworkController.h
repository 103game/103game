#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF


#include <queue>
#include <exception>

#include <zmq.hpp>
#include <zhelpers.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "JSONMessage.h"



#define NTWK_STATE_SWITCH_TIME 1 // set state time (seconds)

using namespace std;
using namespace boost;

extern boost::mutex threadLocker;

class Server; // Server.h уже включает в себя этот h, поэтому делаем так, чтобы не было рекурсии


typedef enum {
	NTWK_LOOP_STATE_RECEIVE, NTWK_LOOP_STATE_SEND
} NTWK_LOOP_STATE;


class NetworkController
{
	public:

	Server *server;
	long long ticks;

	NTWK_LOOP_STATE networkLoopState;	
		
	queue<JSONMessage> receivedMessages; 	
	queue<JSONMessage> messagesToSend; 

	void switchState() {
		if(networkLoopState == NTWK_LOOP_STATE_RECEIVE){
			networkLoopState = NTWK_LOOP_STATE_SEND;
		} else {
			networkLoopState = NTWK_LOOP_STATE_RECEIVE;
		}

		cout << "STATE changed to " << (networkLoopState == NTWK_LOOP_STATE_RECEIVE?"RECEIVE":"SEND") << endl;
	}



	JSONMessage immediateReply(JSONMessage req) {
	
		cout << "Action " + req.getAction() << endl;
	
		if(req.getAction() == "signup"){		
			this->receivedMessages.push(req);	
			return JSONMessage::ok(req.getClientId());
		} else if(req.getAction() == "signin") {			
			return JSONMessage::ok(req.getClientId());
		} else {
			cout << "Unknown action in immediate reply " + req.getAction() << endl;
			return JSONMessage::error("Unknown action sent to server \""+req.getAction()+"\"", req.getClientId());
		}
	}



	static void networkMainLoop(NetworkController *ntw)
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
			
				if(ntw->messagesToSend.size()) {
					// if we have something to send clients

					// get next reply from reply queue
					JSONMessage rep = ntw->messagesToSend.front();
					cout << "Sending message: " << rep.getString() << endl;
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


	NetworkController(Server *_server)
	{
		this->server = _server;
		this->ticks = 0;

		boost::thread networkThread(networkMainLoop, this);	
	}
		
};

#endif