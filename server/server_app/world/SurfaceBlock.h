
#ifndef SURFACEBLOCK_CLASS_DEF
#define SURFACEBLOCK_CLASS_DEF

#include <vector>
#include "WorldObject.h"


typedef enum {
	SURFACE_GRASS, SURFACE_SAND, SURFACE_LAVA,
	SURFACE_SNOW, SURFACE_ROCK, SURFACE_DIRT
} SURFACE ;

typedef struct {
	long x;
	long y;
} COORDS;

class SurfaceBlock{
	public: 
		SURFACE surfaceType;
		COORDS coords;
		bool respawn;
		bool safe;
		std::vector<WorldObject *> blockObjects;
};

#endif