#include "CompilerOptions.h"
#include "NetworkController.h"

#include "Server.h"

#include <boost/thread/thread.hpp>

#include <zmq.hpp>
#include <zhelpers.hpp>

#include <exception>

#include "Utils.h"

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;
extern boost::condition_variable receivedMessagesCond, messagesToSendCond;

void NetworkController::switchState() {
	if(networkLoopState == NTWK_LOOP_STATE_RECEIVE){
		networkLoopState = NTWK_LOOP_STATE_SEND;
	} else {
		networkLoopState = NTWK_LOOP_STATE_RECEIVE;
	}

	//cout << "STATE changed to " << (networkLoopState == NTWK_LOOP_STATE_RECEIVE?"RECEIVE":"SEND") << endl;
}



void NetworkController::messageReceiver(JSONMessage req) {
	
	cout << "Action " + req.getAction() << endl;

	clock_t start_wait_for_unclock = clock();

	{
		boost::lock_guard<boost::mutex> lock(receivedMessagesMutex);
		Utils::LOG("RCV_UNLCK_WAIT_TIME: "+to_string((clock() - start_wait_for_unclock)/(double) CLOCKS_PER_SEC));
		this->receivedMessages.push(req);
		/*Utils::LOG("NOTIFY RCVD MSGS");
		receivedMessagesCond.notify_one();*/
	}		
}



void NetworkController::networkMainLoop(NetworkController *ntw)
{
	// Setup sockets
	zmq::context_t context(1);	
	// router socket
	zmq::socket_t responder(context, ZMQ_ROUTER);

	// set timeout for 1 sec
	/*int timeout = 1; 
	responder.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
	responder.setsockopt(ZMQ_SNDTIMEO, &timeout, sizeof(timeout));*/

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
				
			clock_t receive_start = clock();
			// receive client's address
			string address = s_recvf (responder, ZMQ_NOBLOCK);
			if(address.size()){
				// if we have clients
				// receive message
				string message = s_recv (responder);	

				if(message.size()) {
					cout << "Received "+message+" from " + address << endl;						
					// create immediate reply with created request	
					clock_t msg_receiver_start = clock();
					ntw->messageReceiver(JSONMessage(message, address));
					Utils::LOG("RCV_TIME: "+to_string((clock() - msg_receiver_start)/(double) CLOCKS_PER_SEC));

				}
			}							

			//Utils::LOG("SINGLE_RCV_TIME: "+to_string((clock() - receive_start)/(double) CLOCKS_PER_SEC));
			
			
				
		}else if (ntw->networkLoopState == NTWK_LOOP_STATE_SEND) {
			

			if(ntw->messagesToSend.size()) {
				clock_t start_snd = clock();
				boost::lock_guard<boost::mutex> lock(messagesToSendMutex);
				Utils::LOG("SND_LOCK_TIME: "+to_string((clock() - start_snd)/(double) CLOCKS_PER_SEC));
				
				JSONMessage rep = ntw->messagesToSend.front();
				cout << "Sending message " /*<< rep.getString()*/ << endl;
				Utils::LOG("SND_COPYMSG__TIME: "+to_string((clock() - start_snd)/(double) CLOCKS_PER_SEC));
				s_sendmoref (responder, rep.getClientId(), ZMQ_NOBLOCK);				
				s_sendf (responder, rep.toString(), ZMQ_NOBLOCK);

				
				ntw->messagesToSend.pop();
				Utils::LOG("SND_TIME: "+to_string((clock() - start_snd)/(double) CLOCKS_PER_SEC));
			}
		}

		// check if switch needed
		double state_seconds = (clock() - last_state_started)/(double) CLOCKS_PER_SEC;
		if(state_seconds > NTWK_STATE_SWITCH_TIME) {
			//Utils::LOG("NTWK_"+string((ntw->networkLoopState == NTWK_LOOP_STATE_RECEIVE?"RECEIVE":"SEND"))+"_TIME: "+to_string(state_seconds));
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
		