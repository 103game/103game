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

		BSONObj root;

		string action;
		BSONObj params;

		string clientId;
		string session_id;

		public:

		JSONMessage(){
			construct(BSON("action" << "none" << "params" << BSONObj()), "none");
		}
		
		JSONMessage(string _jsonString, string _clientId){
			construct(fromjson(_jsonString), _clientId);
		}

		JSONMessage(BSONObj obj, string _clientId){
			construct(obj, _clientId);
		}

		void construct(BSONObj obj, string _clientId){
			try{
				root = obj;
				action = root.getStringField("action");
				params = root.getField("params").Obj().getOwned();

				clientId = _clientId;

				if(params.hasField("session_id")){
					session_id = params.getStringField("session_id");
					Utils::LOG("session id exist: "+session_id);
				}else{
					session_id = "";
				}

			}catch(exception &e){
				Utils::ERR("failed to parse json"+string(e.what()));
			} 
		}
		

		static JSONMessage actionmsg(string action, BSONObj params, string clientId) {	
			

			BSONObj obj = mongo::BSONObjBuilder().append("action", action)
								.append("params", params).obj();
			
			return JSONMessage(obj, clientId);
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

		string getSessionId(){
			return session_id;
		}

		shared_ptr<User> getUser(){
			if(getSessionId() != ""){
				return UserMapper::getBySessionId(getSessionId());
			}

			return NULL;
		}

		string toString() {
			return root.toString();
		}

		string getAction() {
			return action;
		}

		BSONObj getParams() {
			return params;
		}



		


		
};

#endif