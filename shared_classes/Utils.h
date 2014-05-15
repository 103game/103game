#ifndef UTILS_CLASS_DEF
#define UTILS_CLASS_DEF

#include "CompilerOptions.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>

#ifdef CLIENT_APP
#ifdef WIN32
#include <winsock2.h>
#include <Windows.h>
#endif
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
		//logged.push_back(s);
		STR_OUT(s);
	}


	static void ERR(string s){
		LOG("Error: "+s);
	}

	static std::string randomString( size_t length )
	{
		static unsigned int i = 0;
		srand( (unsigned)time( NULL ) + i );
		i++;
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

	/*
	Convert tm structure to string
	Interprets the contents of the tm structure pointed by timeptr as a calendar time 
	and converts it to a C-string containing a human-readable version of the corresponding 
	date and time.
	*/
	char* asctime(const struct tm *timeptr)
	{
		static const char wday_name[][4] = {
		"Sun", "Mon", "Tue", 
		"Wed", "Thu", "Fri", 
		"Sat"
	};
	static const char mon_name[][4] = {
		"Jan", "Feb", "Mar", 
		"Apr", "May", "Jun",
		"Jul", "Aug", "Sep", 
		"Oct", "Nov", "Dec"
	};
		static char result[26];
		sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
		wday_name[timeptr->tm_wday],
		mon_name[timeptr->tm_mon],
		timeptr->tm_mday, timeptr->tm_hour,
		timeptr->tm_min, timeptr->tm_sec,
		1900 + timeptr->tm_year);
		return result;
	}

};


#endif
