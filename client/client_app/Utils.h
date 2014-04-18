#ifndef UTILS_CLASS_DEF
#define UTILS_CLASS_DEF

#include <string>
#include <algorithm>
#include <sstream>

 #define DBOUT( s ){ std::wostringstream os_; os_ << s << "\n"; OutputDebugStringW( os_.str().c_str() ); } 


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

};

#endif