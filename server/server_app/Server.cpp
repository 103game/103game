#include <iostream>
#include <boost/thread/thread.hpp>


#include "Server.h"


void serverMainLoop(Server *server)
{
	while(true)
	{		
		server->ticks++;
	}
}

Server::Server()
{
	// �������������� ����
	this->networkController = new NetworkController(this);
	// �������������� ���� ������
	this->dbController = new DBController(this);
	this->dbController->connect(); // connect

	this->ticks = 0;



	boost::thread mainLoop(serverMainLoop, this); // start server main loop	
	mainLoop.join();
}

