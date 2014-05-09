#ifndef OBJECTACTIONMOVE_CLASS_DEF
#define OBJECTACTIONMOVE_CLASS_DEF

#include "ObjectAction.h"

#ifdef SERVER_APP
#include "World.h"
extern shared_ptr<World> sharedWorld;
#endif

class ObjectActionMove: public ObjectAction {

protected:
	DIRECTION direction;
	shared_ptr<SurfaceBlock> moveTo;

public:

	ObjectActionMove(shared_ptr<WorldObject> _obj, double _duration, DIRECTION _direction){
		construct(_obj, _duration, _direction);
	}

	void construct(shared_ptr<WorldObject> _obj = NULL, double _duration = 0, DIRECTION _direction = DIRECTION_UP ){
		ObjectAction::construct(_obj, _duration);
		direction = _direction;

#ifdef SERVER_APP
		if(!sharedWorld->canGo(object, direction)){
			finished = true;
			return;
		}
		
		COORDS crd = object->getSurfaceBlock()->getCoords();

		switch(direction){
		case DIRECTION_UP:
			moveTo = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x, crd.y-1));			
			break;
		case DIRECTION_DOWN:
			moveTo = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x, crd.y+1));			
			break;
		case DIRECTION_LEFT:
			moveTo = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x-1, crd.y));			
			break;
		case DIRECTION_RIGHT:
			moveTo = sharedWorld->getSurfaceBlockByCoords(COORDS(crd.x+1, crd.y));			
			break;
		}
#endif
	}

#ifdef SERVER_APP
	void fin(){
		ObjectAction::fin();
		sharedWorld->move(object, moveTo);
	}
#endif

};



#endif