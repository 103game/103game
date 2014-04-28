#ifndef SERIALIZABLE_CLASS_DEF
#define SERIALIZABLE_CLASS_DEF

#include <string>

#include <bson/bson.h>
using namespace bson;

using namespace std;


class Serializable {
	
private:
	string className;

public:

	Serializable () {
		setClassName("Serializable");
	}

	BSONObj toBSON() {
		BSONObjBuilder builder;
		builder
			.append("class", getClassName());

		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		setClassName(obj.getStringField("class"));
	}
	

	string getClassName(){return className;}
	void setClassName(string _className){className = _className;}
};




#endif