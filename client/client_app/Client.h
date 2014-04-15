#ifndef CLIENT_CLASS_DEF
#define CLIENT_CLASS_DEF

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>


#include "NetworkController.h"
#include "ClientActions.h"
#include "JSONMessage.h"

extern boost::mutex receivedMessagesMutex, messagesToSendMutex;
extern boost::condition_variable receivedMessagesCond, messagesToSendCond;

class ClientApp;


class Client {
	public:		
		NetworkController *networkController;
		ClientApp *app;
		ClientActions *clientActions;
		long long ticks;

		Client(ClientApp *_app) {
			this->app = _app;
			this->networkController = new NetworkController(this);
			this->ticks = 0;
		}

		void mainLoop() {
			
			boost::unique_lock<boost::mutex> lock(receivedMessagesMutex);
			//receivedMessagesCond.wait(lock);
			
			if(this->networkController->receivedMessages.size()){
				JSONMessage req = this->networkController->receivedMessages.front();							
				this->clientActions->messageForwarder(req);

				this->networkController->receivedMessages.pop();
				
			}else{
				//DBOUT("NO INCOMING MESSAGES");
			}
			

			this->ticks++;
		} 

};

#endif
