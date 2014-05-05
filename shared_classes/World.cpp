#include "CompilerOptions.h"
#include "World.h"
#include "Creatures.h"

#include <boost/thread.hpp>


void World::move(shared_ptr<WorldObject> obj, shared_ptr<SurfaceBlock> to) {	
	if(obj->surfaceBlock != NULL){
		obj->surfaceBlock->clear();
	}

	if(!to->isEmpty()){
		cout << "MOVED TO NON-EMPTY BLOCK. CAN BE MEM LEAK" << endl;
		to->clear();
	}

	to->setObject(obj);
	obj->surfaceBlock = to;
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


