#ifndef USER_CLASS_DEF
#define USER_CLASS_DEF

#include "Common.h"

using namespace std;

class User {

	public:		

		User(string _email, string _password, string _name);
		User(string _id);

		void saveUserToDb();	

	
		string id;
		string email;
		string password_md5;
		string name;
		string session_id;
		
};

#endif