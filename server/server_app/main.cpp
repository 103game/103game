#include "CompilerOptions.h"

#include "Server.h"

#include <boost/thread/thread.hpp>

boost::mutex receivedMessagesMutex, messagesToSendMutex;

int main(void)
{
	Server server;

	return 0;
}