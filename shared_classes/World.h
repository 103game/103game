#ifndef WORLD_CLASS_DEF
#define WORLD_CLASS_DEF

#include <vector>

#include "SurfaceBlock.h"

#include "Serializable.h"

#include "BSON.h"

#include <unordered_map>


using namespace std;

class World : Serializable
{
	public: 		
		unordered_map<COORDS, shared_ptr<SurfaceBlock>, COORDSHasher> sbMap;


		World() {
			setClassName("World");
		}

		void move(shared_ptr<WorldObject> obj, shared_ptr<SurfaceBlock> to);
		shared_ptr<SurfaceBlock> World::getSurfaceBlockByCoords(COORDS coords);

		
		

		BSONObj toBSON() {
			BSONObjBuilder builder;

			BSONArrayBuilder arrBuilder;
			for(unordered_map<COORDS, shared_ptr<SurfaceBlock>, COORDSHasher>::iterator it = sbMap.begin(); it != sbMap.end(); it++) {
				arrBuilder.append(it->second->toBSON());
			}

			builder.appendElements(Serializable::toBSON())
				.append("blocks", arrBuilder.arr());

			return builder.obj();
		}

		void fromBSON(BSONObj obj) {
			Serializable::fromBSON(obj);		
			
			vector<BSONElement> arr = obj["blocks"].Array();

			for (vector<BSONElement>::iterator it = arr.begin(); it != arr.end(); ++it){
				BSONObj block = it->Obj();
				

			}
			

		}
};

#endif