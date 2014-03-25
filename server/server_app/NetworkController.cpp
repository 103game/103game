#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>

#include "NetworkController.h"



void networkMainLoop(NetworkController *ntw)
{
	while(true)
	{		
		ntw->tickCount++;
	}
}


NetworkController::NetworkController(Server *_server)
{
	this->server = _server;
	this->tickCount = 0;

	Json::Value root;
	Json::Reader reader;
	
	if(reader.parse("{\"type\": \"class\"}", root)){
		std::string type = root.get("type", "not_class").asString();	
		std::cout << type << std::endl;

	}else{
		std::cout << "Error parsing JSON" << std::endl;
	}

	
	boost::thread networkThread(networkMainLoop, this);
	
}