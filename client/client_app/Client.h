#ifndef CLIENT_CLASS_DEF
#define CLIENT_CLASS_DEF


#include "NetworkController.h"

class ClientApp;

class Client {
	public:		
		NetworkController *networkController;
		ClientApp *app;

		Client(ClientApp *_app);

};

#endif
