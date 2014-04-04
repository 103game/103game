#ifndef JSONMESSAGE_CLASS_DEF
#define JSONMESSAGE_CLASS_DEF

#include <json/json.h>
#include <exception>

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
		
		JSONMessage(string json_str):jsonString(json_str) {
			Json::Reader reader;

			if (!reader.parse(jsonString, root)){					
				throw std::exception(reader.getFormattedErrorMessages().c_str());
			}

			if(!root.get("action", false).asBool()) {
				throw exception("Wrong JSON format: no \"action\" parameter");
			}

			action = root.get("action", false).asString();

			if(!root.get("params", false).asBool()) {
				throw exception("Wrong JSON format: no \"params\" parameter");
			}

			params = root.get("params", false);
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