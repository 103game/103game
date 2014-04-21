#ifndef JSONMESSAGE_CLASS_DEF
#define JSONMESSAGE_CLASS_DEF

#include <json/json.h>
#include <exception>

#include "Utils.h"

using namespace std;


/*

	Message structure
		- action: actionName
		- params: params

*/


class JSONMessage {

		string jsonString;

		Json::Value root;

		string action;
		Json::Value params;

		public:
		
		JSONMessage(string _jsonString):jsonString(_jsonString){
			static Json::Reader reader;		

			if (!reader.parse(jsonString, root)){					
				throw std::exception(reader.getFormattedErrorMessages().c_str());
			}			
		
			if(!root["action"].asString().size()) {
				throw std::exception("Wrong JSON format: no1 \"action\" parameter ");
			}
			

			action = root.get("action", false).asString();
			

			if(!root["params"]) {
				throw std::exception("Wrong JSON format: no \"params\" parameter");
			}

			params = root.get("params", false);
			
		}

		bool hasErrors(){
			return !params.get("errors", false).isNull();
		}



		string getString() {
			return jsonString;
		}

		string getAction() {
			return action;
		}

		Json::Value getParams() {
			return params;
		}



		


		
};

#endif