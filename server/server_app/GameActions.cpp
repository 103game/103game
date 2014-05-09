#include "GameActions.h"

#include "Server.h"
#include "World.h"

#include "JSONMessage.h"
#include "Creatures.h"

void GameActions::updateObjects(){
	shared_ptr<World> world = server->world;

	for(vector<shared_ptr<WorldObject>>::iterator it = world->objects.begin(); it != world->objects.end(); it++){
		(*it)->update();
	}
}


void GameActions::control(JSONMessage msg) {
	string ctrl = msg.getParams().getStringField("control");

	shared_ptr<World> world = server->world;
	auto user = msg.getUser();
	shared_ptr<WorldObject> creature = world->getWorldObjectById(user->getCreatureId());	

	if(creature == NULL || !world->isOnMap(creature)){
		return;
	}
		

	Utils::LOG(creature->toBSON().jsonString());
	Utils::LOG(creature->getSurfaceBlock()->toBSON().jsonString());

	if(ctrl == "go_up"){
		world->move(creature, DIRECTION_UP);
	}else if(ctrl == "go_down"){
		world->move(creature, DIRECTION_DOWN);
	}else if(ctrl == "go_right"){
		world->move(creature, DIRECTION_RIGHT);
	}else if(ctrl == "go_left"){
		world->move(creature, DIRECTION_LEFT);
	}

}

