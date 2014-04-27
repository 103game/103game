#ifndef CREATURES_DEF
#define CREATURES_DEF

#include "WorldObject.h"
#include "User.h"

#include <mongo/bson/bson.h>

using namespace mongo;


class Creature: public WorldObject {

private:
	int life;

	bool bot;
	User *user;

public:

	Creature() {
		setClassName("Creature");
		bot = true;
		life = 100;
		user = NULL;
	}

	BSONObj toBSON(){
		BSONObjBuilder builder;
		builder.appendElements(WorldObject::toBSON())
					.append("life", life)
					.append("bot", bot);

		if(!bot){
			builder.append("user", user!=NULL?user->getId():"null");
		}

		return builder.obj();
	}

	int getLife(){return life;}
	void setLife(int _life){life = _life;}

	bool isBot(){return bot;}
	void setBot(bool _bot){bot = _bot;}

	User* getUser(){return user;}
	void setUser(User *_user){user = _user;}
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

	string getZombieType(){return zombieType;}
	void setZombieType(string _zombieType){zombieType = _zombieType;}
};


#endif