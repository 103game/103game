#ifndef USER_CLASS_DEF
#define USER_CLASS_DEF

#include "DBObject.h"

#include <string>
#include <sstream>
#include "Utils.h"


#include "DBController.h"

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
			this->db_collection = USERS_DB_COLLECTION;
			this->id = "";
			this->email = _email;	
			this->password_md5 = Utils::md5(_password);
			this->name = _name;
		}

		User(string _id, string _email, string _password_md5, string _name, string _session_id):

			email(_email), password_md5(_password_md5), name(_name), session_id(_session_id)
		{
			this->id = _id;
			this->db_collection = USERS_DB_COLLECTION;
		}
		
		mongo::BSONObj toBSON () {			
			mongo::BSONObjBuilder builder;
			builder.append("email", this->email)
				.append("password_md5", this->password_md5)
				.append("name", this->name)
				.append("session_id", this->session_id);

			return builder.obj();
		}
		

		static bool emailTaken(string email){
			return !sharedDb->getObjectByQuery(USERS_DB_COLLECTION, QUERY("email" << email)).isEmpty();
		}

		static User getUserByEmailAndPassword(string email, string password) {
			mongo::BSONObj obj = sharedDb->getObjectByQuery(USERS_DB_COLLECTION, QUERY(
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
			mongo::BSONObj obj = sharedDb->getObjectById(USERS_DB_COLLECTION, mongo::OID(id));

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