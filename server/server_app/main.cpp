#define _CRT_SECURE_NO_WARNINGS

#include "ServerAppController.h"

#include <boost/thread/mutex.hpp>

boost::mutex threadLocker;

int main(void)
{
	ServerAppController serverApp;
	serverApp.initServer();

	return 0;
}
