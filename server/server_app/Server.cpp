#include <iostream>
#include <boost/thread/thread.hpp>


#include "Server.h"

#include <boost/thread/mutex.hpp>

extern boost::mutex threadLocker;


void serverMainLoop(Server *server)
{
	while(true)
	{		
		// cook responses
		//threadLocker.lock();
		NetworkController *ntw = server->networkController;

		if(ntw->receivedMessages.size()) {
			AddressedRequest req = ntw->receivedMessages.front();
			AddressedReply rep("You are mfucker no. "+req.senderId, req.senderId);

			ntw->cookedMessages.push(rep);
			ntw->receivedMessages.pop();
		}
		

		//threadLocker.unlock();

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

