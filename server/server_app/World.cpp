#include "World.h"
#include "Creatures.h"

void World::move(WorldObject *obj, SurfaceBlock *to) {	
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

SurfaceBlock* World::getSurfaceBlockByCoords(COORDS coords) {
	for(int i = 0; i < surfaceBlocks.size(); i++){
		SurfaceBlock *block = &surfaceBlocks[i];
		if(block->getCoords() == coords){
			return block;
		}
	}
	return NULL;
}


