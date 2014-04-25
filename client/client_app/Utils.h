#ifndef UTILS_CLASS_DEF
#define UTILS_CLASS_DEF


#include <string>
#include <algorithm>
#include <sstream>
#include <time.h>

#include <winsock2.h>
#include <Windows.h>

using namespace std;

class Utils{

public:

	static std::string randomString( size_t length )
	{
		srand( (unsigned)time( NULL ) );
		auto randchar = []() -> char
		{
			const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[ rand() % max_index ];
		};
		std::string str(length,0);
		std::generate_n( str.begin(), length, randchar );
		return str;
	}

	static void log(string s){		
		std::wostringstream os_;
		os_ << s.c_str() << endl;
		OutputDebugStringW(os_.str().c_str());
	}

};

#endif