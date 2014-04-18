#ifndef UTILS_CLASS_DEF
#define UTILS_CLASS_DEF

#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>

#include "MD5.h"

#include <mongo/client/dbclient.h>



 #define DBOUT( s ){ std::wostringstream os_; os_ << s << "\n"; OutputDebugStringW( os_.str().c_str() ); } 

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

	static std::string md5(std::string str) {
		static MD5 md5;
		return md5.digestString((char *)str.c_str());
	}

	static bool isCharacter(const char Character){
		return ( (Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
	}

	static bool isNumber(const char Character){
		return ( Character >= '0' && Character <= '9');
	}

	static bool isValidEmailAddress(string email)
	{
		if(!email.length())
			return 0;
		if(!isCharacter(email[0]))
			return 0;
		int AtOffset = -1;
		int DotOffset = -1;
		unsigned int Length = email.length();

		for(unsigned int i = 0; i < Length; i++)
		{
			if(email[i] == '@')
				AtOffset = (int)i;
			else if(email[i] == '.')
				DotOffset = (int)i;
		}

		if(AtOffset == -1 || DotOffset == -1)
			return 0;
		if(AtOffset > DotOffset)
			return 0;
		return !(DotOffset >= ((int)Length-1));
	}

};


#endif
