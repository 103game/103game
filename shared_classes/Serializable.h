#ifndef SERIALIZABLE_CLASS_DEF
#define SERIALIZABLE_CLASS_DEF

#include <string>

#include "BSON.h"
#include "Utils.h"

using namespace std;


class Serializable {
	
private:
	string className;
	string id;

public:

	Serializable () {
		setClassName("Serializable");
		id = Utils::randomString(20); 
	}

	BSONObj toBSON() {
		BSONObjBuilder builder;
		builder
			.append("id", id)
			.append("class", getClassName());

		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		setClassName(obj.getStringField("class"));
		setId(obj.getStringField("id"));		
	}
	

	string getClassName(){return className;}
	void setClassName(string _className){className = _className;}

	string getId(){ return id; }
	void setId(string _id){ id = _id; }
};




#endif