#ifndef CREATURES_DEF
#define CREATURES_DEF

#include "WorldObject.h"
#include "User.h"

#include <mongo/bson/bson.h>

#define NO_USER "no_user"


class Creature: public WorldObject {

private:
	int life;

	bool bot;
	string userId;

public:

	Creature() {
		setClassName("Creature");
		bot = true;
		life = 100;
		userId = NO_USER;
	}

	BSONObj toBSON(){
		BSONObjBuilder builder;
		builder.appendElements(WorldObject::toBSON())
					.append("life", life)
					.append("bot", bot);

		if(!bot && userId != NO_USER){
			builder.append("user", getUserId());
		}

		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		WorldObject::fromBSON(obj);

		setLife(obj.getIntField("life"));
		setBot(obj.getBoolField("bot"));		

		if(!isBot()){			
			if(obj.hasField("user")){
				setUserId(obj.getStringField("user"));
			}else{
				setUserId(NO_USER);
			}
		}
	}

	int getLife(){return life;}
	void setLife(int _life){life = _life;}

	bool isBot(){return bot;}
	void setBot(bool _bot){bot = _bot;}

	string getUserId(){return userId;}
	void setUserId(string _userId){userId = _userId;}
};

class Survivor: public Creature {
	
public:
	
	Survivor() {
		setClassName("Survivor");
	}

	BSONObj toBSON(){
		BSONObjBuilder builder;
		builder
			//.append("class", getClassName())
			.appendElements(Creature::toBSON());

		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		Creature::fromBSON(obj);		
	}
};

class Zombie: public Creature {

private:
	string zombieType;

public:
	Zombie() {
		setClassName("Zombie");
		setZombieType("simple");
	}

	BSONObj toBSON(){
		BSONObjBuilder builder;
		builder			
			.appendElements(Creature::toBSON())
			.append("zombieType", getZombieType());

		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		Creature::fromBSON(obj);
		setZombieType(obj.getStringField("zombieType"));			
	}

	string getZombieType(){return zombieType;}
	void setZombieType(string _zombieType){zombieType = _zombieType;}
};


#endif