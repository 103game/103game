#ifndef USER_CLASS_DEF
#define USER_CLASS_DEF

#include "DBObject.h"

#include <string>
#include <sstream>
#include "Utils.h"

#include <bson/bson.h>
using namespace bson;

#define USERS_DB_COLLECTION "server.users"

using namespace std;


class User:public DBObject {

private:
	string email;
	string password_md5;
	string name;
	string session_id;


public:	


	User(string _email = "", string _password = "", string _name = "")
	{
		this->setDbCollection(USERS_DB_COLLECTION);		
		this->setClassName("User");

		this->email = _email;	
		this->password_md5 = Utils::md5(_password);
		this->name = _name;
		this->session_id = "";
	}

	BSONObj toBSON () {			
		BSONObjBuilder builder;
		builder.appendElements(DBObject::toBSON())
			.append("email", this->email)
			.append("password_md5", this->password_md5)
			.append("name", this->name)
			.append("session_id", this->session_id);

		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		DBObject::fromBSON(obj);
		setEmail(obj.getStringField("email"));
		setName(obj.getStringField("name"));
		setPasswordMd5(obj.getStringField("password_md5"));
		setSessionId(obj.getStringField("session_id"));		
	}

	string getEmail(){return email;}
	void setEmail(string _email){email = _email;}
	string getName(){return name;}
	void setName(string _name){name = _name;}
	string getPasswordMd5(){return password_md5;}
	void setPasswordMd5(string _password_md5){password_md5 = _password_md5;}
	string getSessionId(){return session_id;}
	void setSessionId(string _session_id){session_id = _session_id;}

};

#endif