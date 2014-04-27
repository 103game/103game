#ifndef USER_CLASS_DEF
#define USER_CLASS_DEF

#include "DBObject.h"

#include <string>
#include <sstream>
#include "Utils.h"

#include <mongo/bson/bson.h>

#define USERS_DB_COLLECTION "server.users"

using namespace std;


class User:public DBObject {

	public:	
		string email;
		string password_md5;
		string name;
		string session_id;

		User(string _email = "", string _password = "", string _name = "")
		{
			this->setDbCollection(USERS_DB_COLLECTION);		
			this->setClassName("User");

			this->email = _email;	
			this->password_md5 = Utils::md5(_password);
			this->name = _name;
		}

		User(string _id, string _email, string _password_md5, string _name, string _session_id):

			email(_email), password_md5(_password_md5), name(_name), session_id(_session_id)
		{
			this->setId(_id);
			this->setDbCollection(USERS_DB_COLLECTION);
		}
		
		BSONObj toBSON () {			
			BSONObjBuilder builder;
			builder.append("email", this->email)
				.append("password_md5", this->password_md5)
				.append("name", this->name)
				.append("session_id", this->session_id);

			return builder.obj();
		}
};

#endif