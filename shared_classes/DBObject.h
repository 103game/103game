#ifndef DBOBJECT_CLASS_DEF
#define DBOBJECT_CLASS_DEF

#include "Serializable.h"

#include <string>

#include "Utils.h"

using namespace std;
using namespace mongo;

class DBObject:public Serializable {

private:
	string db_collection;
	string id;

public:

	DBObject() {
		setClassName("DBObject");
		id = "not_in_db_"+Utils::randomString(5); // before saving to db
		db_collection = "";
	}

	BSONObj toBSON(){
		BSONObjBuilder builder;
		builder
			.appendElements(Serializable::toBSON());
		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		Serializable::fromBSON(obj);
		setId(obj.getStringField("id"));
	}

	bool isInDb() { 
		return id.substr(0, 9) != "not_in_db";
	}	

	string getDbCollection(){return db_collection;}
	void setDbCollection(string _db_collection){db_collection = _db_collection;}

	string getId(){ return id; }
	void setId(string _id){ id = _id; }
};

#endif