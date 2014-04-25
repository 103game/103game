
#pragma warning(disable: 4018)

#include "ClientApp.h"

#include <boost/thread/thread.hpp>

boost::mutex receivedMessagesMutex, messagesToSendMutex;

