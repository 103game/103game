
#ifndef DBCONTROLLER_CLASS_DEF
#define DBCONTROLLER_CLASS_DEF

#define STATIC_LIBMONGOCLIENT
#include <mongo/client/dbclient.h>



#include <string>

using namespace std;
using namespace mongo;

class Server;
class DBObject;

class DBController {
	public:

	shared_ptr<DBClientConnection> c;
	Server *server;	

	DBController(Server *_server):server(_server){}

	bool connect();

	bool objectExists(string collection, string id);

	void DBController::saveObject(shared_ptr<DBObject> obj);

	mongo::BSONObj getObjectById(string collection, string id);
	mongo::BSONObj getObjectByQuery(string collection, mongo::Query query, string sort = "");
	vector<mongo::BSONObj> getObjectsByQuery(string collection, string json_query, string sort = "");
	vector<mongo::BSONObj> getObjectsByQuery(string collection, mongo::Query query, string sort = "");
	
	void update(string collection, string json_where, string json_how, bool create_if_no = false, bool multi = true);
	void update(string collection, mongo::BSONObj where, mongo::BSONObj how, bool create_if_no = false, bool multi = true);
	void insert(string collection, mongo::BSONObj obj);
	void insert(string collection, string json);

};





#endif