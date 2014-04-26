#define _CRT_SECURE_NO_WARNINGS
#define STATIC_LIBMONGOCLIENT
#pragma warning(disable: 4251 4275 4244 4267) // disable special warnings from mongo


#include "Server.h"

#include <boost/thread/thread.hpp>

boost::mutex receivedMessagesMutex, messagesToSendMutex;

int main(void)
{
	Server server;

	return 0;
}