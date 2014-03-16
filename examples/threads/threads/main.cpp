#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <iostream>

void threadFunc1()
{
	// mission: output a dot every second

	while(1){
		std::cout << ".";

		// pretend to do something
		boost::posix_time::seconds workTime(1); 
		boost::this_thread::sleep(workTime);  

		// point where thread can be interrupted by another
		boost::this_thread::interruption_point();
	}
}

void  threadFunc2(boost::thread *one)
{
	// mission: sleep 5 seconds then kill thread 1

	boost::posix_time::seconds workTime(5); 
	boost::this_thread::sleep(workTime); 

	
	// finish him!	
	one->interrupt();

	std::cout << "\nFirst thread was finished by second one" << std::endl;
}



int main(void)
{
	// construct and run threads 
	boost::thread one(threadFunc1); 
	boost::thread two(threadFunc2, &one);  // pass first thread as an argument

	// say to MAIN thread wait for our threads to finish
	one.join();
	two.join();

	return 0;
}