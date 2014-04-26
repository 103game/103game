#include "DBController.h"
#include "Server.h"


#include "Utils.h"


bool DBController::connect()
{
	try {
			
		this->c = new mongo::DBClientConnection();
		this->c->connect("localhost");

	} catch(const mongo::DBException &e) {			
		DBOUT(string("Error: "+string(e.what())).c_str());
		return false;
	}

	return true;
}

bool DBController::objectExists(string collection, mongo::OID id){
	return !this->getObjectById(collection, id).isEmpty();
}

mongo::BSONObj DBController::getObjectById(string collection, mongo::OID id) {
	return this->getObjectByQuery(collection, QUERY("_id" << id));
}

mongo::BSONObj DBController::getObjectByQuery(string collection, mongo::Query query, string sort) {
	vector<mongo::BSONObj> objects;
	objects = this->getObjectsByQuery(collection, query, sort);
	if(objects.size())
		return objects[0];
	return mongo::BSONObj(); // empty object (isEmpty() to check)
}

vector<mongo::BSONObj> DBController::getObjectsByQuery(string collection, string json_query, string sort) {
	return this->getObjectsByQuery(collection, mongo::Query(json_query), sort);
}

vector<mongo::BSONObj> DBController::getObjectsByQuery(string collection, mongo::Query query, string sort) {
	vector<mongo::BSONObj> result;
	auto_ptr<mongo::DBClientCursor> cursor;
	if(sort != ""){
		cursor = this->c->query(collection, query.sort(sort));
	}else{
		cursor = this->c->query(collection, query);
	}
		
	while(cursor->more()){
		mongo::BSONObj obj = cursor->next();

		mongo::BSONObjBuilder builder;
			
		mongo::BSONElement oi;
		obj.getObjectID(oi);
		mongo::OID oid = oi.__oid();

		obj = builder.appendElements(obj).append("id", oid.toString()).obj();
			
			
		result.push_back(obj.getOwned());
	}

	return result;
}

	
void DBController::update(string collection, string json_where, string json_how, bool create_if_no, bool multi) {
	this->update(collection, mongo::fromjson(json_where), mongo::fromjson(json_how), create_if_no, multi);
}

void DBController::update(string collection, mongo::BSONObj where, mongo::BSONObj how, bool create_if_no, bool multi) {
	this->c->update(collection, where, how, create_if_no, multi);
}

mongo::OID DBController::insert(string collection, mongo::BSONObj obj) {
	mongo::BSONObjBuilder builder;
	obj = builder.appendElements(obj).genOID().obj();

	this->c->insert(collection, obj);
	mongo::BSONElement oi;
	obj.getObjectID(oi);
	mongo::OID oid = oi.__oid();		
		
	return oid;
}

mongo::OID DBController::insert(string collection, string json) {
	mongo::BSONObj obj = mongo::fromjson(json);

	return this->insert(collection, obj);
}


