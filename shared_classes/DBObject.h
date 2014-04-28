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
	bool inDb;

public:

	DBObject() {
		setClassName("DBObject");
		id = Utils::randomString(20); 
		db_collection = "";
		setInDb(false);
	}

	BSONObj toBSON(){
		BSONObjBuilder builder;
		builder
			.appendElements(Serializable::toBSON())
			.append("id", id)
			.append("inDb", inDb);
		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		Serializable::fromBSON(obj);
		setId(obj.getStringField("id"));
	}

	bool isInDb() { 
		return inDb;
	}	

	void setInDb(bool _inDb) {
		inDb = _inDb;
	}

	string getDbCollection(){return db_collection;}
	void setDbCollection(string _db_collection){db_collection = _db_collection;}

	string getId(){ return id; }
	void setId(string _id){ id = _id; }
};

#endif