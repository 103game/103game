#include "GameActions.h"

#include "Server.h"
#include "World.h"

void GameActions::respawnObject(shared_ptr<WorldObject> wo){

	if(server->world->isOnMap(wo))
		return;

	auto map = this->server->world->sbMap;

	for(unordered_map<COORDS, shared_ptr<SurfaceBlock>, COORDSHasher>::iterator it = map.begin(); it != map.end(); it++){
		shared_ptr<SurfaceBlock> sb = it->second;
		if(sb->getObject() == NULL){
			server->world->move(wo, sb);
			break;
		}
	}
}
