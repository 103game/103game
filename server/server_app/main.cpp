/*
/*
* This application is a MMORPG app which is suitable
* for all people, who prefer really excellent and
* exciting games for your PC. The main feature of
* our application is your opportunity to be a part
* of a huge world and take part in its evolution
* during your all "virtual" life.
*
* It is the first Russian serious project that can 
* can get at the global industrial gaming market, 
* just because our primary responsibility is 
* safety of all your data. We are convinced that
* it is a main problem of our days.
* 
* @authors: George Fedoseev, Andrew Penkrat, 
* Vitaly Mozin, Anna Fatkina, Ulyana Minaeva, 
* Darya Legankova
* Saint-Petersburg, Russia
* Saint Petersburg State University
* PM-PU Department
* 2014 (c)
*/

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