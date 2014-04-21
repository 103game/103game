#ifndef DBOBJECT_CLASS_DEF
#define DBOBJECT_CLASS_DEF

#include "Serializable.h"
#include "DBController.h"

#include <string>

using namespace std;

extern DBController *sharedDb;



class DBObject:public Serializable {

protected:
	string db_collection;

public:
	string id;

	void saveToDb(){
		if(id == "") {
			mongo::OID id = sharedDb->insert(this->db_collection, this->toBSON().getOwned());
			this->id = id.toString();
		}else {
			sharedDb->update(this->db_collection, BSON("_id" << id), this->toBSON());
		}			
	}
};

#endif