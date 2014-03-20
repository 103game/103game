
#ifndef WORLD_CLASS_DEF
#define WORDL_CLASS_DEF

#include <vector>

#include "SurfaceBlock.h"
#include "WorldObject.h"

typedef enum{
	DAYTIME_MORNING, DAYTIME_DAY, DAYTIME_EVENING, DAYTIME_NIHGT
} DAYTIME;

class World
{
	public: 
		DAYTIME daytime; // время дня
		std::vector<std::vector<SurfaceBlock *>> surfaceBlocks; // собственно карта
		std::vector<WorldObject *> worldObjects; // объекты на карте

};

#endif