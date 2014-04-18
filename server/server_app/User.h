#ifndef USER_CLASS_DEF
#define USER_CLASS_DEF

#include <string>
#include <sstream>
#include "Utils.h"

#include "DBController.h"

#define USERS_DB_COLLECTION "server.users"

using namespace std;

extern DBController *sharedDb;

class User {

	public:
		
		string id;
		string email;
		string password_md5;
		string name;
		string session_id;

		User(string _email, string _password, string _name)
		{
			this->id = "";
			this->email = _email;	
			this->password_md5 = Utils::md5(_password);
			this->name = _name;
		}

		string toJSON() {
			mongo::BSONObjBuilder builder;
			mongo::BSONObj obj =  builder.append("email", this->email).append("name", this->name).genOID().obj();
			return obj.jsonString();
		}

		void saveToDb(){
			if(id == "") {

			}else {

			}			
		}

		

		static User getUserById(string id){

		}
		
};

#endif