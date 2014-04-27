#ifndef WORLD_CLASS_DEF
#define WORLD_CLASS_DEF

#include <vector>

#include "SurfaceBlock.h"

#include "Serializable.h"



using namespace std;

class World : Serializable
{
	public: 		
		vector<SurfaceBlock> surfaceBlocks;	


		World() {
			setClassName("World");
		}

		void move(WorldObject *obj, SurfaceBlock *to);
		SurfaceBlock* World::getSurfaceBlockByCoords(COORDS coords);

		
		

		BSONObj toBSON() {
			BSONObjBuilder builder;

			BSONArrayBuilder arrBuilder;
			for(int i = 0; i < surfaceBlocks.size(); i++) {
				arrBuilder.append(surfaceBlocks[i].toBSON());
			}

			builder.appendElements(Serializable::toBSON())
				.append("blocks", arrBuilder.arr());

			return builder.obj();
		}
};

#endif