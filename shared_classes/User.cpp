#include "User.h"
#include "Creatures.h"

#ifdef SERVER_APP

shared_ptr<Creature> User::getCreature(){
	if(creatureId != NO_CREATURE){
		BSONObj obj = sharedDb->getObjectById("server.worldobjects", creatureId);
		if(!obj.isEmpty()){
			shared_ptr<Creature> crt = shared_ptr<Creature>(new Creature());
			crt->fromBSON(obj);
			return crt;
		}
	}

	return NULL;
}

#endif