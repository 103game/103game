
#ifndef DBCONTROLLER_CLASS_DEF
#define DBCONTROLLER_CLASS_DEF


#include <string>
#include <mongo/client/dbclient.h>
#include <boost/shared_ptr.hpp>

#include "Utils.h"

using namespace std;




class Server;

class DBController{
	public:
	boost::shared_ptr<mongo::DBClientConnection> c;
	Server *server;	

	DBController(Server *_server)
	{
		this->server = _server;
	}

	bool connect()
	{
		try {
			
			this->c = boost::shared_ptr<mongo::DBClientConnection>(new mongo::DBClientConnection());
			this->c->connect("localhost");
				
			

		/*	mongo::OID insert_id = this->insert("server.hello", "{\"name\": \"Bob\", \"age\": 20}");
			mongo::BSONObj obj = this->getObjectByQuery("server.hello", QUERY("age" << 20));
			cout << this->objectExists("server.hello", insert_id) << endl;*/

		} catch(const mongo::DBException &e) {			
			DBOUT(string("Error: "+string(e.what())).c_str());
			return false;
		}

		return true;
	}

	bool objectExists(string collection, mongo::OID id){
		return !this->getObjectById(collection, id).isEmpty();
	}

	mongo::BSONObj getObjectById(string collection, mongo::OID id) {
		return this->getObjectByQuery(collection, QUERY("_id" << id));
	}

	mongo::BSONObj getObjectByQuery(string collection, mongo::Query query, string sort = "") {
		vector<mongo::BSONObj> objects;
		objects = this->getObjectsByQuery(collection, query, sort);
		if(objects.size())
			return objects[0];
		return mongo::BSONObj(); // empty object (isEmpty() to check)
	}

	vector<mongo::BSONObj> getObjectsByQuery(string collection, string json_query, string sort = "") {
		return this->getObjectsByQuery(collection, mongo::Query(json_query), sort);
	}

	vector<mongo::BSONObj> getObjectsByQuery(string collection, mongo::Query query, string sort = "") {
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

	
	void update(string collection, string json_where, string json_how, bool create_if_no = false, bool multi = true) {
		this->c->update(collection, mongo::fromjson(json_where), mongo::fromjson(json_how), create_if_no, multi);
	}

	mongo::OID insert(string collection, mongo::BSONObj obj) {
		this->c->insert(collection, obj);
		mongo::BSONElement oi;
		obj.getObjectID(oi);
		mongo::OID oid = oi.__oid();		
		
		return oid;
	}

	mongo::OID insert(string collection, string json) {
		mongo::BSONObj obj = mongo::fromjson(json);
		mongo::BSONObjBuilder builder;
		obj = builder.appendElements(obj).genOID().obj();

		return this->insert(collection, obj);
	}



};





#endif