#include "CompilerOptions.h"

#include "Server.h"

#include <boost/thread/thread.hpp>

boost::mutex receivedMessagesMutex, messagesToSendMutex;
boost::condition_variable receivedMessagesCond, messagesToSendCond;

int main(void)
{
	Server server;

	return 0;
}