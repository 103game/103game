#ifndef USERMAPPER_CLASS_DEF
#define USERMAPPER_CLASS_DEF

#include "User.h"

#include "DBController.h"

extern DBController *sharedDb;

class UserMapper {

public:
	static bool emailTaken(string email){
		return !sharedDb->getObjectByQuery(USERS_DB_COLLECTION, QUERY("email" << email)).isEmpty();
	}

	static User getUserByEmailAndPassword(string email, string password) {
		BSONObj obj = sharedDb->getObjectByQuery(USERS_DB_COLLECTION, QUERY(
			"email" << email << "password_md5" << Utils::md5(password)
			));


		if(obj.isEmpty()) {
			return User();
		}

		return User(
			obj.getStringField("id"),
			obj.getStringField("email"),
			obj.getStringField("password_md5"),
			obj.getStringField("name"),
			obj.getStringField("session_id")
			);
	}



	static User getById(string id) {
		BSONObj obj = sharedDb->getObjectById(USERS_DB_COLLECTION, OID(id));

		if(obj.isEmpty()){
			return User();
		}

		return User(
			obj.getStringField("id"),
			obj.getStringField("email"),
			obj.getStringField("password_md5"),
			obj.getStringField("name"),
			obj.getStringField("session_id")
			);

	}
};

#endif