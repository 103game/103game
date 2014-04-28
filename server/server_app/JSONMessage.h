#ifndef JSONMESSAGE_CLASS_DEF
#define JSONMESSAGE_CLASS_DEF

#ifdef WIN32
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif

#include <exception>

#define STATIC_LIBMONGOCLIENT
#include <mongo/bson/bson.h>


using namespace std;
using namespace mongo;

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

			return JSONMessage(BSON("action" << "ok" << "params" << BSONObj()).jsonString(), clientId);
		}

		static JSONMessage actionmsg(string action, mongo::BSONObj params, string clientId) {	
			

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