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

		User(string _id, string _email, string _password_md5, string _name, string _session_id):
			id(_id), email(_email), password_md5(_password_md5), name(_name), session_id(_session_id)
		{}

		string toJSON() {
			mongo::BSONObj obj = this->toBSON().copy();
			{
			 auto a = obj.toString(true);
			 obj.dump();
			}

		}

		mongo::BSONObj toBSON() {
			mongo::BSONObjBuilder builder;
			mongo::BSONObj obj =  builder.append("email", this->email)
									.append("password_md5", this->password_md5)
									.append("name", this->name)	
									.append("session_id", this->session_id)
									.obj();			
			return obj;
		}






		static bool emailTaken(string email){
			return !sharedDb->getObjectByQuery("server.users", QUERY("email" << email)).isEmpty();
		}



		static User getById(string id) {
			mongo::BSONObj obj = sharedDb->getObjectById("server.users", mongo::OID(id));
			return User(
					obj.getStringField("_id"),
					obj.getStringField("email"),
					obj.getStringField("password_md5"),
					obj.getStringField("name"),
					obj.getStringField("session_id")
				);

		}


		void saveToDb(){
			if(id == "") {
				mongo::OID id = sharedDb->insert("server.users", this->toBSON());
				this->id = id.toString();
			}else {
				sharedDb->update("server.users", BSON("_id" << id), this->toBSON());
			}			
		}

		

		static User getUserById(string id){

		}
		
};

#endif