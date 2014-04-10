#ifndef JSONMESSAGE_CLASS_DEF
#define JSONMESSAGE_CLASS_DEF

#ifdef WIN32
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif

#include <exception>


using namespace std;


/*

	Message structure
		- action: actionName
		- params: params

*/


class JSONMessage {

		string jsonString;
		string clientId;

		Json::Value root;

		string action;
		Json::Value params;

		public:
		
		JSONMessage(string _jsonString, string _clientId):jsonString(_jsonString), clientId(_clientId){
			Json::Reader reader;

			if (!reader.parse(jsonString, root)){					
				//throw std::exception(reader.getFormattedErrorMessages().c_str());
			}
		
			if(!root["action"].asString().size()) {
				//throw std::exception("Wrong JSON format: no action parameter");
			}

			action = root.get("action", false).asString();

			if(!root["params"]) {
				//throw std::exception("Wrong JSON format: no params parameter");
			}

			params = root.get("params", false);
		}
		


		string getClientId(){
			return clientId;
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