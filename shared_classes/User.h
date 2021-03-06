#ifndef USER_CLASS_DEF
#define USER_CLASS_DEF

#include "CompilerOptions.h"

#include "DBObject.h"

#include <string>
#include <sstream>
#include "Utils.h"


#include "BSON.h"

#ifdef SERVER_APP
#include "DBController.h"
extern DBController *sharedDb;
#endif

#define USERS_DB_COLLECTION "server.users"
#define NO_CREATURE "no_creature"


using namespace std;

class Creature;


/* User
* class with:
* 5 private fields
* 14 public functions
*/


class User:public DBObject {

private:
	string email;
	string password_md5;
	string name;
	string session_id;
	string creatureId;

public:	


	User(string _email = "", string _password = "", string _name = "") // data from user
	{
		this->setDbCollection(USERS_DB_COLLECTION);		
		this->setClassName("User");

		this->email = _email;	
		this->password_md5 = Utils::md5(_password);
		this->name = _name;
		this->session_id = "";
		this->creatureId = NO_CREATURE;
	}

	BSONObj toBSON () {			
		BSONObjBuilder builder;
		builder.appendElements(DBObject::toBSON())
			.append("email", this->email)
			.append("password_md5", this->password_md5)
			.append("name", this->name)
			.append("session_id", this->session_id)
			.append("creatureId", this->creatureId);

		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		DBObject::fromBSON(obj);
		setEmail(obj.getStringField("email")); //setting Email
		setName(obj.getStringField("name")); //setting Name
		setPasswordMd5(obj.getStringField("password_md5")); //setting Password in md5 format
		setSessionId(obj.getStringField("session_id")); //setting SessionId
		setCreatureId(obj.getStringField("creatureId")); //setting CreatureId
	}

	string getEmail(){return email;} //getting Email
	void setEmail(string _email){email = _email;} //Setting Email
	string getName(){return name;} //getting Name
	void setName(string _name){name = _name;} //setting Name
	string getPasswordMd5(){return password_md5;} //getting Password in md5 format
	void setPasswordMd5(string _password_md5){password_md5 = _password_md5;} //setting Password in md5 format
	string getSessionId(){return session_id;} //getting SessionId
	void setSessionId(string _session_id){session_id = _session_id;} //setting SessionId
	string getCreatureId(){return creatureId;} //getting CreatureID
	void setCreatureId(string _creatureId){creatureId = _creatureId;} //setting CreatureId

	shared_ptr<Creature> getCreature();



};

#endif