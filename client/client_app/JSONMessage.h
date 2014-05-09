#ifndef JSONMESSAGE_CLASS_DEF
#define JSONMESSAGE_CLASS_DEF

#include <exception>
#include "Utils.h"

#include "BSON.h"

using namespace std;


/*

	Message structure
		- action: actionName
		- params: params

*/


class JSONMessage {

private:
		string jsonString;

		BSONObj root;		
		string action;
		BSONObj params;
		string session_id;

public:

		JSONMessage(){
			construct(BSON("action" << "none" << "params" << BSONObj()));
		}
		
		JSONMessage(string _jsonString, string _ssid = ""){			
			jsonString = _jsonString;
			session_id = _ssid;
		}

		void parse(){
			construct(fromjson(jsonString), session_id);
		}

		JSONMessage(BSONObj obj, string _ssid = ""){			
			construct(obj, _ssid);
		}

		void construct(BSONObj _obj, string _ssid = ""){
			try{
				root = _obj;
				action = root.getStringField("action");
				params = root.getField("params").Obj().getOwned();

				session_id = _ssid;

				// add ssid to params
				params = BSONObjBuilder().appendElements(params).append("session_id", session_id).obj();

				root = BSONObjBuilder().append("action", action).append("params", params).obj();

				if(hasErrors()){
					Utils::LOG(getErrorsString());
				}

			}catch(exception &e){
				Utils::ERR("failed to parse json"+string(e.what()));
			}
		}

		bool hasErrors(){
			return params.hasField("errors");
		}

		vector<string> getErrors(){
			vector<string> errVec;
			if(this->hasErrors()) {
				vector<BSONElement> errors = params["errors"].Array();				
				for(int i = 0; i < errors.size(); i++){
					errVec.push_back(errors[i].String());
				}
			}
			return errVec;			
		}

		string getErrorsString(){			
			stringstream ss;
			vector<string> errors = getErrors();	
			ss << "Action " << action << " Errors:" << endl;
			if(errors.size()){				
				for(int i = 0; i < errors.size(); i++){					
					ss << " - " << errors[i] << endl;
				}
			}else{
				ss << "  No errors" << endl;
			}
			return ss.str();			
		}

		string toString(){
			return root.jsonString();
		}

		string getAction() {
			return action;
		}

		BSONObj getParams() {
			return params;
		}



		


		
};

#endif