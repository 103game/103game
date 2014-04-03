#ifndef ADDRESSEDREQUEST_CLASS_DEF
#define ADDRESSEDREQUEST_CLASS_DEF

#include <string>

using namespace std;

class AddressedRequest {
	public:
		AddressedRequest(string _msg, string _sndid):messageString(_msg), senderId(_sndid){};

		string messageString;
		string senderId;
};

#endif