#ifndef PUBLISHERMESSAGE_CLASS_DEF
#define PUBLISHERMESSAGE_CLASS_DEF

#include <string>

using namespace std;

class PublisherMessage {
	public:

		PublisherMessage(string _msg, string _subid):messageString(_msg), subscriberId(_subid){};

		string messageString;
		string subscriberId;



};


#endif