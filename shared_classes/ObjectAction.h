#ifndef OBJECTACTION_CLASS_DEF
#define OBJECTACTION_CLASS_DEF

#include "CompilerOptions.h"
#include "Serializable.h"

#include "WorldObject.h"

#include <boost/lexical_cast.hpp>


using namespace std;

class ObjectAction: public Serializable {

protected:
	double duration, currentTime;
	clock_t started;

	bool finished;

	shared_ptr<WorldObject> object;

public:

	ObjectAction() {
		construct();
	}

	ObjectAction(shared_ptr<WorldObject> _obj, double _duration) {
		construct(_obj, _duration);
	}

	virtual void construct(shared_ptr<WorldObject> _obj = NULL, double _duration = 0) {		
		setClassName("ObjectAction");
		duration = _duration;
		object = _obj;
		started = 0;
		finished = false;		
	}

	BSONObj toBSON(){
		BSONObjBuilder builder;
		builder.appendElements(Serializable::toBSON())
			.append("duration", to_string(duration))
			.append("currentTime", to_string(currentTime));

		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		Serializable::fromBSON(obj);	
		duration = boost::lexical_cast<double>(obj.getStringField("duration"));
		currentTime = boost::lexical_cast<double>(obj.getStringField("currentTime"));
	}

	bool isFinished(){return finished;}

#ifdef SERVER_APP

	virtual void update(){
		if(!started){
			started = clock();
		}
		currentTime = (clock() - started)/(double) CLOCKS_PER_SEC;
		//Utils::LOG("update "+to_string(currentTime)+"/"+to_string(duration));

		if(currentTime > duration){
			fin();				
			return;
		}
	}

	virtual void fin(){		
		finished = true;
	}

#endif

};

#endif