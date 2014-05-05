#include "CompilerOptions.h"
#include "World.h"
#include "Creatures.h"

#include <boost/thread.hpp>


bool World::moveRight(shared_ptr<WorldObject> obj){
	COORDS crd = obj->getSurfaceBlock()->getCoords();	
	return move(obj, COORDS(crd.x+1, crd.y));
}

bool World::moveLeft(shared_ptr<WorldObject> obj){
	COORDS crd = obj->getSurfaceBlock()->getCoords();	
	return move(obj, COORDS(crd.x-1, crd.y));
}

bool World::moveUp(shared_ptr<WorldObject> obj){
	COORDS crd = obj->getSurfaceBlock()->getCoords();	
	return move(obj, COORDS(crd.x+1, crd.y));
}

bool World::moveDown(shared_ptr<WorldObject> obj){
	COORDS crd = obj->getSurfaceBlock()->getCoords();	
	return move(obj, COORDS(crd.x+1, crd.y));
}

bool World::move(shared_ptr<WorldObject> obj, COORDS to) {	
	auto sb = getSurfaceBlockByCoords(to);
	if(sb != NULL){
		move(obj, sb);
		return true;
	}
	return false;
}

bool World::move(shared_ptr<WorldObject> obj, shared_ptr<SurfaceBlock> to) {	

	if(to == NULL || !to->isEmpty())
		return false;

	if(obj->surfaceBlock != NULL){
		obj->surfaceBlock->clear();
	}

	to->setObject(obj);
	obj->surfaceBlock = to;

	return true;
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
	//boost::lock_guard<boost::mutex> lock(worldMutex);
	if(sbMap.size()){
		unordered_map<COORDS, shared_ptr<SurfaceBlock>, COORDSHasher>::iterator it = sbMap.find(coords);
		if(it != sbMap.end()){
			return it->second;
		}
	}	
	return NULL;
}


