#include "CompilerOptions.h"
#include "World.h"
#include "Creatures.h"

#include <boost/thread.hpp>

#ifdef SERVER_APP
extern shared_ptr<World> sharedWorld;
#endif


#ifdef SERVER_APP

/* function canGo
	* returns 1, if the necessary square of the field is empty
	* returns 0, if it is not empty
*/

bool World::canGo(shared_ptr<WorldObject> wo, DIRECTION dir) {	 
	auto sb = getSbFrom(wo, dir);
	return sb != NULL && sb->isEmpty();
}

shared_ptr<SurfaceBlock> World::getSbFrom(COORDS crd, DIRECTION dir){	
	shared_ptr<SurfaceBlock> sb;
	switch(dir){
	case DIRECTION_UP: // go up
		sb = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x, crd.y-1));			
		break;
	case DIRECTION_DOWN: // go down
		sb = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x, crd.y+1));			
		break;
	case DIRECTION_LEFT: // go left
		sb = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x-1, crd.y));			
		break;
	case DIRECTION_RIGHT: // go right
		sb = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x+1, crd.y));			
		break;
	}
	return sb;
}

vector<shared_ptr<WorldObject>> World::getWorldObjectsAround(shared_ptr<WorldObject> wo) {
	vector<shared_ptr<WorldObject>> woVec;

	if(wo){
		shared_ptr<SurfaceBlock> sb;
		for(int i = DIRECTION_UP; i <= DIRECTION_LEFT; i++){
			if(sb = getSbFrom(wo, (DIRECTION)i)){
				shared_ptr<WorldObject> sbWo = sb->getObject();
				if(sbWo){
					woVec.push_back(sbWo);
				}
			}
		}	
	}
		
	return woVec;
}

shared_ptr<SurfaceBlock> World::getSbFrom(shared_ptr<WorldObject> wo, DIRECTION dir){	
	COORDS crd = wo->getSurfaceBlock()->getCoords();
	return getSbFrom(crd, dir);
}


bool World::move(shared_ptr<WorldObject> obj, DIRECTION to) {	
	if(canGo(obj, to)){
		auto sb = getSbFrom(obj, to);		
		move(obj, sb);
		return true;		
	}
	
	return false;
}


bool World::move(shared_ptr<WorldObject> obj, COORDS to) {	
	shared_ptr<SurfaceBlock> sb = getSurfaceBlockByCoords(to);
	if(sb != NULL){
		move(obj, sb);
		return true;
	}
	return false;
}

bool World::move(shared_ptr<WorldObject> obj, shared_ptr<SurfaceBlock> to) {	

	if(!isOnMap(obj)){
		objects.push_back(obj);
	}

	if(to == NULL || !to->isEmpty())
		return false;

	if(obj->getSurfaceBlock() != NULL)
		Utils::LOG("move from "+obj->getSurfaceBlock()->getCoords().toString()+" to "+to->getCoords().toString());

	if(obj->surfaceBlock != NULL){
		obj->surfaceBlock->clear();
	}

	
	

	obj->surfaceBlock = to;
	to->setObject(obj);
	

	return true;
}

void World::remove(shared_ptr<WorldObject> wo) {
	shared_ptr<SurfaceBlock> sb = wo->getSurfaceBlock();
	if(sb != NULL){
		sb->setObject(NULL);
	}

	for(vector<shared_ptr<WorldObject>>::iterator it = objects.begin(); it != objects.end(); it++){
		if(wo->getId() == (*it)->getId()){
			objects.erase(it);
			break;
		}
	}

}

shared_ptr<WorldObject> World::getWorldObjectById(string id) {
	for(vector<shared_ptr<WorldObject>>::iterator it = objects.begin(); it != objects.end(); it++){
		if(id == (*it)->getId()){
			return *it;			
		}
	}

	return NULL;
}

shared_ptr<WorldObject> World::respawnObject(shared_ptr<WorldObject> wo){

	if(isOnMap(wo))
		return getWorldObjectById(wo->getId());

	for(unordered_map<COORDS, shared_ptr<SurfaceBlock>, COORDSHasher>::iterator it = sbMap.begin(); it != sbMap.end(); it++){
		shared_ptr<SurfaceBlock> sb = it->second;
		if(sb->getObject() == NULL){
			move(wo, sb);
			return wo;
			break;
		}
	}

	return NULL;
}

#endif

/* function isOnMap
	* returns 1, if the user is on map
	* returns 0, if it is not on map
*/

bool World::isOnMap(shared_ptr<WorldObject> wo) {
	for(vector<shared_ptr<WorldObject>>::iterator it = objects.begin(); it != objects.end(); it++){
		if(wo->getId() == (*it)->getId()){
			return true;
		}
	}
	return false;
}

void World::insertSb(shared_ptr<SurfaceBlock> sb){
	this->sbMap.insert(
			pair<COORDS, shared_ptr<SurfaceBlock>>(
			sb->getCoords(),
			sb
		)
	);
}

shared_ptr<SurfaceBlock> World::getSurfaceBlockByCoords(COORDS coords) {
	if(sbMap.size()){

		unordered_map<COORDS, shared_ptr<SurfaceBlock>, COORDSHasher>::iterator it = sbMap.find(coords);
		if(it != sbMap.end()){
			return it->second;
		}		
	}	
	return NULL;
}


