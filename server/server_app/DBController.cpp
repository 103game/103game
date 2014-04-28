#include "CompilerOptions.h"

#include "DBController.h"
#include "Server.h"


#include "DBObject.h"

#include "User.h"
#include "Creatures.h"

#include "Utils.h"

using namespace mongo;

bool DBController::connect()
{
	try {
			
		this->c = make_shared<DBClientConnection>();
		this->c->connect("localhost");

	} catch(const mongo::DBException &e) {			
		Utils::ERR(e.what());
		return false;
	}

	return true;
}

void DBController::saveObject(DBObject &obj){
	bool newRecord = !obj.isInDb();	
	obj.setInDb(true);

	BSONObj bson;
	if(obj.getClassName() == "User"){
		bson = ((User*)&obj)->toBSON();
	}else if(obj.getClassName() == "Survivor"){
		bson = ((Survivor*)&obj)->toBSON();
	}else if(obj.getClassName() == "Zombie"){
		bson = ((Zombie*)&obj)->toBSON();
	}else{
		Utils::ERR("Save object: unknown DBObject - "+obj.getClassName());
	}

	if(newRecord) {				
		this->insert(obj.getDbCollection(), bson);		
	}else {		
		this->update(obj.getDbCollection(), BSON("id" << obj.getId()), bson);
	}	
}


bool DBController::objectExists(string collection, mongo::OID id){
	return !this->getObjectById(collection, id).isEmpty();
}

mongo::BSONObj DBController::getObjectById(string collection, mongo::OID id) {
	return this->getObjectByQuery(collection, QUERY("id" << id));
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

void DBController::insert(string collection, mongo::BSONObj obj) {
	mongo::BSONObjBuilder builder;
	obj = builder.appendElements(obj).obj();
	this->c->insert(collection, obj);	
}

void DBController::insert(string collection, string json) {
	mongo::BSONObj obj = mongo::fromjson(json);
	this->insert(collection, obj);
}


