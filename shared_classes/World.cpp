#include "CompilerOptions.h"
#include "World.h"
#include "Creatures.h"

#include "ObjectActionMove.h"

#include <boost/thread.hpp>


#ifdef SERVER_APP

bool World::canGo(shared_ptr<WorldObject> wo, DIRECTION dir) {
	shared_ptr<SurfaceBlock> sb;
	COORDS crd = wo->getSurfaceBlock()->getCoords();

	switch(dir){
		case DIRECTION_UP:
			sb = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x, crd.y-1));			
			break;
		case DIRECTION_DOWN:
			sb = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x, crd.y+1));			
			break;
		case DIRECTION_LEFT:
			sb = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x-1, crd.y));			
			break;
		case DIRECTION_RIGHT:
			sb = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x+1, crd.y));			
			break;
	}

	return sb != NULL && sb->isEmpty();
}


void World::moveRight(shared_ptr<WorldObject> obj){	
	obj->addAction(shared_ptr<ObjectActionMove>(new ObjectActionMove(obj, 1, DIRECTION_RIGHT)));
}

void World::moveLeft(shared_ptr<WorldObject> obj){
	obj->addAction(shared_ptr<ObjectActionMove>(new ObjectActionMove(obj, 1, DIRECTION_LEFT)));
}

void World::moveUp(shared_ptr<WorldObject> obj){
	obj->addAction(shared_ptr<ObjectActionMove>(new ObjectActionMove(obj, 1, DIRECTION_UP)));
}

void World::moveDown(shared_ptr<WorldObject> obj){
	obj->addAction(shared_ptr<ObjectActionMove>(new ObjectActionMove(obj, 1, DIRECTION_DOWN)));
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


