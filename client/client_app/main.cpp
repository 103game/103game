#include "CompilerOptions.h"

#include "ClientApp.h"

#include <boost/thread/thread.hpp>

boost::mutex receivedMessagesMutex, messagesToSendMutex;

