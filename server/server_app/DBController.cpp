

#include "DBController.h"

DBController::DBController(Server *_server)
{
	this->server = _server;
}

void DBController::connect()
{
	try {
		/*this->connection = new DBClientConnection();
		this->connection->connect("localhost");
		
		cout << "connected!" << endl;
		*/
		// EXAMPLE:

		/*
		// create object
		BSONObjBuilder obj;		
		obj.append( "num" , 1);
		obj.append( "age" , 18);				
		
		// insert to db
		this->connection->insert("server.people", obj.obj());
		*/

	} catch(const DBException &e) {
		cout << "DBConnect exception" << e.what() << endl;
	}
}