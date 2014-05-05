#ifndef JSONMESSAGE_CLASS_DEF
#define JSONMESSAGE_CLASS_DEF

#include <exception>
#include "BSON.h"

#include "Utils.h"

#include "UserMapper.h"


using namespace std;


/*

	Message structure
		- action: actionName
		- params: params

*/
class JSONMessage;

class JSONMessage {

		string jsonString;
		string clientId;

		BSONObj root;

		string action;
		BSONObj params;

		public:

		JSONMessage(){

		}
		
		JSONMessage(string _jsonString, string _clientId):jsonString(_jsonString), clientId(_clientId){
			try{
				root = fromjson(jsonString);
				action = root.getStringField("action");
				params = root.getField("params").Obj().getOwned();


			}catch(exception &e){
				Utils::ERR("failed to parse json"+string(e.what()));
			}

		}
		

		static JSONMessage actionmsg(string action, BSONObj params, string clientId) {	
			

			BSONObj obj = mongo::BSONObjBuilder().append("action", action)
								.append("params", params).obj();
			
			return JSONMessage(obj.jsonString(), clientId);
		}

		static JSONMessage error (string action, string err, string clientId) {
			vector<string> errors;
			errors.push_back(err);			
			return JSONMessage::errors(action, errors, clientId);
		}

		static JSONMessage errors(string action, vector<string> errors, string clientId){
			mongo::BSONArrayBuilder arrBuilder;

			for(int i = 0; i < errors.size(); i++){
				arrBuilder.append(errors[i]);
			}
			
			BSONObj params = BSON("errors" << arrBuilder.arr());

			return JSONMessage::actionmsg(action, params, clientId);
		}
		


		string getClientId(){
			return clientId;
		}

		User getUser(){

		}

		string getString() {
			return jsonString;
		}

		string getAction() {
			return action;
		}

		BSONObj getParams() {
			return params;
		}



		


		
};

#endif