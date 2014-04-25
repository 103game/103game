#ifndef SERIALIZABLE_CLASS_DEF
#define SERIALIZABLE_CLASS_DEF

#include <string>
#include <mongo/client/dbclient.h>

using namespace std;

class Serializable {
	
public:
	virtual mongo::BSONObj toBSON() = 0;

	string toJSON() {
		return this->toBSON().jsonString();
	}
};




#endif