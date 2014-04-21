#ifndef SERIALIZABLE_CLASS_DEF
#define SERIALIZABLE_CLASS_DEF

#include <string>
#include <mongo/client/dbclient.h>

using namespace std;

class Serializable {
	
public:
	virtual string toJSON() = 0;

	mongo::BSONObj toBSON() {			
		return mongo::fromjson(this->toJSON());
	}
};




#endif