#ifndef WORLD_CLASS_DEF
#define WORLD_CLASS_DEF

#include <vector>

#include "SurfaceBlock.h"

#include "Serializable.h"

#include "BSON.h"

#include <unordered_map>
#include <vector>

#include "WorldObject.h"


using namespace std;

class World : Serializable
{
	public: 		
		unordered_map<COORDS, shared_ptr<SurfaceBlock>, COORDSHasher> sbMap;
		vector<shared_ptr<WorldObject>> objects;


		World() {
			setClassName("World");
			//Utils::LOG("sbSize:" + to_string(sbMap.size()));
		}


		void insertSb(shared_ptr<SurfaceBlock> sb);

		bool World::canGo(shared_ptr<WorldObject> wo, DIRECTION dir);

		void moveRight(shared_ptr<WorldObject> obj);
		void moveLeft(shared_ptr<WorldObject> obj);
		void moveUp(shared_ptr<WorldObject> obj);
		void moveDown(shared_ptr<WorldObject> obj);

		bool move(shared_ptr<WorldObject> obj, COORDS to);
		bool move(shared_ptr<WorldObject> obj, shared_ptr<SurfaceBlock> to);

		bool World::isOnMap(shared_ptr<WorldObject> wo);
		void World::remove(shared_ptr<WorldObject> wo);

		shared_ptr<WorldObject> World::getWorldObjectById(string id);
		shared_ptr<WorldObject> World::respawnObject(shared_ptr<WorldObject> wo);


		shared_ptr<SurfaceBlock> getSurfaceBlockByCoords(COORDS coords);


		
		

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

			sbMap.clear();

			//Utils::LOG("world serialized size: " + to_string(arr.size()));

			for (vector<BSONElement>::iterator it = arr.begin(); it != arr.end(); ++it){
				BSONObj block = it->Obj();
				shared_ptr<SurfaceBlock> sb = shared_ptr<SurfaceBlock>(new SurfaceBlock());
				sb->fromBSON(block);
				//Utils::LOG("unsrlzng: "+sb->toBSON().jsonString());
				sbMap.insert(pair<COORDS, shared_ptr<SurfaceBlock>>(sb->getCoords(), sb));
			}

			//Utils::LOG("world unserializeD size: " + to_string(sbMap.size()));
			

		}
};

#endif