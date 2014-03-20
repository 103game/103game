
#ifndef WORLDOBJECT_CLASS_DEF
#define WORLDOBJECT_CLASS_DEF

class SurfaceBlock; // SurfaceBlock.h уже включает в себя этот h, поэтому делаем так, чтобы не было рекурсии

class WorldObject{

	public:
		SurfaceBlock *surfaceBlock;

};

#endif