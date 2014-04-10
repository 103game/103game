/*
This is the whole world.
*/

#ifndef WORLD_CLASS_DEF
#define WORLD_CLASS_DEF

#include <vector>

#include "SurfaceBlock.h"
#include "WorldObject.h"

typedef enum{
	DAYTIME_MORNING, DAYTIME_DAY, DAYTIME_EVENING, DAYTIME_NIHGT
} DAYTIME;

class World
{
	public: 
		DAYTIME daytime;
		std::vector<std::vector<SurfaceBlock *>> surfaceBlocks;
		std::vector<WorldObject *> worldObjects;

};

#endif