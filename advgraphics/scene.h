#pragma once
#include <vector>
#include "shapes3d.h"
#include "gameobj.h"


namespace mgd {

class Scene {
public:
	std::vector<GameObj> gameObjs; // a vector of gameobjects, each with its own transform

	void populate(int numGameObj); // populate the scene
	void decimate();

	void toWorld(std::vector<Sphere>&, std::vector<Plane>&);
	void toView(int idGameObj); // possess selected gameobject

	void transformAll(const Transform&);
	void transformAllLocally(const Transform&);
	void transformJust(int idGameObj, const Transform&);
	
};

void rayCasting(const std::vector<Sphere>& sphereVector, const std::vector<Plane>& planeVector);
float currentTime();

} // end namespace mgd

