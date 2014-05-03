#ifndef UTILS_CLASS_DEF
#define UTILS_CLASS_DEF

#include "CompilerOptions.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>

#ifdef WIN32
#include <winsock2.h>
#include <Windows.h>
#endif

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include "MD5.h"

#include <vector>

using namespace std;

extern vector<string> logged;

class Utils{

public:

	static vector<string> getLastLogged(int number){
		vector<string> ret;

		int loggedSize = (int) logged.size();

		for(int j = (loggedSize-number >=0)?(loggedSize-number):0; j < loggedSize; j++){
			ret.push_back(logged[j]);
		}

		return ret;
	}

	static void STR_OUT(string s){
#ifdef CLIENT_APP
		std::wostringstream os_;
		os_ << s.c_str() << endl;
		OutputDebugStringW(os_.str().c_str());
#else
		cout << s << endl;
#endif
	}

	static void LOG(string s){
		logged.push_back(s);
		STR_OUT(s);
	}


	static void ERR(string s){
		LOG("Error: "+s);
	}

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
		return md5_lib(str);
	}

	// trim from start
	static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	// trim from both ends
	static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
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
