#include "ClientApp.h"

#include <boost/thread/mutex.hpp>

boost::mutex receivedMessagesMutex, messagesToSendMutex;
boost::condition_variable receivedMessagesCond, messagesToSendCond;


