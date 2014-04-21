#define _CRT_SECURE_NO_WARNINGS

#pragma warning(disable: 4251 4275 4244 4267) // disable special warnings from mongo

#include "ServerAppController.h"

#include <boost/thread/mutex.hpp>

boost::mutex receivedMessagesMutex, messagesToSendMutex;
boost::condition_variable receivedMessagesCond, messagesToSendCond;

int main(void)
{
	ServerAppController serverApp;
	serverApp.initServer();

	return 0;
}
