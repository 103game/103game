#include <iostream>
#include "Client.h"


Client::Client(ClientApp *_app)
{	
	this->app = _app;
	this->networkController = new NetworkController(this);
}

