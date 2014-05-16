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

/* class with:
* 3 private fields
* 11 public functions
*/

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

	int getLife(){return life;} // getlife
	void setLife(int _life){life = _life;} //setlife

	void decrLife(int decr){life-=decr; life = life>=0?life:0;}
	/* function isBot
	* returns 1, if it is a bot
	* and returns 0, if it isn't a bot
	*/
	bool isBot(){return bot;} 
	void setBot(bool _bot){bot = _bot;} //setbot

	string getUserId(){return userId;} //get user id and return it
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






/* Creature --- Survivor
* class with:
* 0 private fields
* 5 public functions
*/

class Survivor: public Creature {
	
public:
	
	Survivor() {
		setClassName("Survivor");
	}

#ifdef CLIENT_APP
	void draw(UIRect rect){ //drawing
		Creature::draw(rect);		
	}
#endif

#ifdef SERVER_APP
	void update(){ //updating

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


/* Creature --- Zombie
* class with:
* 1 private fields
* 6 public functions
*/

class Zombie: public Creature {

private:
	string zombieType;

public:
	Zombie() {
		setClassName("Zombie");
		setZombieType("simple");
	}

#ifdef CLIENT_APP	
	void draw(UIRect rect){ //drawing
		Creature::draw(rect);		
	}
#endif

#ifdef SERVER_APP
	void update(){ //updating between Server-Client
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

	string getZombieType(){return zombieType;} //getting the type of Zombie
	void setZombieType(string _zombieType){zombieType = _zombieType;} // setting the type of Zombie
};


#endif