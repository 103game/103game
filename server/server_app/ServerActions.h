#ifndef SERVERACTIONS_CLASS_DEF
#define SERVERACTIONS_CLASS_DEF

#include "JSONMessage.h"

#include "NetworkController.h"

extern boost::mutex threadLocker;

using namespace std;

class ServerActions {
	public:

		NetworkController *ntw;
		
		ServerActions(NetworkController *_ntw):ntw(_ntw){}

		void messageForwarder(JSONMessage msg){
			if(msg.getAction() == "signup"){
				this->signUp(msg);
			}
		}

		void signUp(JSONMessage msg) {
			cout << "SignUp function" << endl;
			threadLocker.lock();
			JSONMessage output("{\"action\": \"signUpCallback\", \"params\": \"ok\"}", msg.getClientId());
			

			ntw->cookedMessages.push(output);
			cout << ntw->cookedMessages.size() << endl;
			threadLocker.unlock();
		}


};


#endif
