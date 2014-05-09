#ifndef DBOBJECT_CLASS_DEF
#define DBOBJECT_CLASS_DEF

#include "Serializable.h"

#include <string>


#include "BSON.h"

using namespace std;


class DBObject:public Serializable {

private:
	string db_collection;	
	bool inDb;

public:

	DBObject() {
		setClassName("DBObject");		
		db_collection = "";
		setInDb(false);
	}

	BSONObj toBSON(){
		BSONObjBuilder builder;
		builder			
			.appendElements(Serializable::toBSON())						
			.append("inDb", inDb);
		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		Serializable::fromBSON(obj);		
		setInDb(obj.getBoolField("inDb"));
	}

	bool isInDb() { 
		return inDb;
	}	

	void setInDb(bool _inDb) {
		inDb = _inDb;
	}

	string getDbCollection(){return db_collection;}
	void setDbCollection(string _db_collection){db_collection = _db_collection;}


};

#endif