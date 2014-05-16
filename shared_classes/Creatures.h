/*


This .h file, which contains information 
about all the characters of the game, 
here are presented the possibility of inheritance 
by which facilitated the task of creating 
a hierarchy of beings. 
Also presented basic methods 
for these classes here.


*/
#ifndef CREATURES_DEF
#define CREATURES_DEF

#include "WorldObject.h"
#include "User.h"

#include "BSON.h"

#include "World.h"

#define NO_USER "no_user"

#ifdef CLIENT_APP
	extern map<string, Texture> sharedTextures;
#endif

#ifdef SERVER_APP
	extern shared_ptr<World> sharedWorld;
#endif

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

	void decrLife(int decr){life-=decr; life = life>=0?life:0;}

	bool isBot(){return bot;}
	void setBot(bool _bot){bot = _bot;}

	string getUserId(){return userId;}
	void setUserId(string _userId){userId = _userId;}

#ifdef CLIENT_APP
	// function of drawing
	void draw(UIRect rect){
		WorldObject::draw(rect);	
		// draw lifebar

		int life = getLife();

		Utils::LOG("DRAW LIFE "+to_string(life));

		UIRect lbRect = UIRect(rect.x+rect.width*.1, rect.y, (life/100.0)*rect.width*.8, 5);	
		if(life > 30){
			color(Color(0, 1, 0));
		}else{
			color(Color(1, 0, 0));
		}

		drawSolidRect(Rectf(lbRect.x, lbRect.y, lbRect.xEnd, lbRect.yEnd));
	}
#endif
	
	
}; // Creature




class Survivor: public Creature {
	
public:
	
	Survivor() {
		setClassName("Survivor");
	}

#ifdef CLIENT_APP
	void draw(UIRect rect){
		Creature::draw(rect);		
	}
#endif

#ifdef SERVER_APP
	void update(){

	}		
#endif

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

#ifdef CLIENT_APP	
	void draw(UIRect rect){
		Creature::draw(rect);		
	}
#endif

#ifdef SERVER_APP
	void update(){ //updating Server-Client
		COORDS crd = getSurfaceBlock()->getCoords();
		shared_ptr<SurfaceBlock> sb;		
		
		vector<shared_ptr<WorldObject>> objectsAround = sharedWorld->getWorldObjectsAround(this->getThisFromSb());

		for(int i = 0; i < objectsAround.size(); i++){
			shared_ptr<WorldObject> wo = objectsAround[i];
			if(wo->getClassName() == "Survivor"){
				static clock_t lastHit = 0;
				if((clock() - lastHit)/(double) CLOCKS_PER_SEC > 1){
					shared_ptr<Survivor> srv = static_pointer_cast<Survivor>(wo);
					srv->decrLife(5);		
					Utils::LOG("Eat srv!");
					Utils::LOG(srv->toBSON().jsonString());
					sharedDb->saveObject(srv);
					wo->surfaceBlock->setObject(srv);
					lastHit = clock();
				}
			}
		}

	}		
#endif

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