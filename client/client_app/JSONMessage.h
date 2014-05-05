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
		string action;
		BSONObj params;

public:

		JSONMessage(){

		}
		
		JSONMessage(string _jsonString):jsonString(_jsonString){
			try{
				BSONObj bson = fromjson(jsonString);
				action = bson.getStringField("action");
				params = bson.getField("params").Obj().getOwned();
				
				
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