#include "GameActions.h"

#include "Server.h"
#include "World.h"

#include "JSONMessage.h"
#include "Creatures.h"


void GameActions::control(JSONMessage msg) {
	string ctrl = msg.getParams().getStringField("control");

	shared_ptr<World> world = server->world;
	auto user = msg.getUser();
	shared_ptr<WorldObject> creature = world->getWorldObjectById(user->getCreatureId());	

	if(creature == NULL || !server->world->isOnMap(creature)){
		return;
	}

	Utils::LOG(creature->toBSON().jsonString());
	Utils::LOG(creature->getSurfaceBlock()->toBSON().jsonString());

	if(ctrl == "go_up"){
		world->moveUp(creature);
	}else if(ctrl == "go_down"){
		world->moveDown(creature);
	}else if(ctrl == "go_right"){
		world->moveRight(creature);
	}else if(ctrl == "go_left"){
		world->moveLeft(creature);
	}

}

