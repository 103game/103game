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
class JSONMessage;

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

		static JSONMessage fromObject(Json::Value val, string clientId){
			Json::StyledWriter writer;
			string jsonString = writer.write(val);
			JSONMessage msg(jsonString, clientId);
			return msg;
		}

		static JSONMessage ok (string clientId) {
			return JSONMessage("{\"action\": \"ok\", \"params\": \"null\"}", clientId);
		}

		static JSONMessage error (string err, string clientId) {
			return JSONMessage("{\"action\": \"error\", \"params\": {\"error\": \""+err+"\"}}", clientId);
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