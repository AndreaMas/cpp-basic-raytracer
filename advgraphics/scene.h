#pragma once
#include <vector>
#include "gameobj.h"

namespace mgd {

class Scene {
public:
	std::vector<GameObj> allGameObjs; // a vector of gameobjects, each with its own transform
	void populate(int numGameobj); // populate allGameObjs with new randomly placed gameobjects
	void toWorld(std::vector<Sphere> &allSpheres) const; // produces a vector of spheres in world space
};

void rayCasting(const std::vector<Sphere>& sphereVector);

} // end namespace mgd

