#ifndef ADDRESSEDREPLY_CLASS_DEF
#define ADDRESSEDREPLY_CLASS_DEF

#include <string>

using namespace std;

class AddressedReply {
	public:

		AddressedReply(string _msg, string _recvid):messageString(_msg), receiverId(_recvid){};

		string messageString;
		string receiverId;

};


#endif