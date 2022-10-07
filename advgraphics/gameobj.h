#pragma once
#include "transform.h"
#include "shapes3d.h"
#include <vector>



namespace mgd {

class GameObj {

public:
	Transform transform;
	std::vector<Sphere> spheres;
	std::vector<Plane> planes;
	
	GameObj();
	virtual void spheresInWorldSpace(std::vector<Sphere>&) const;
	virtual void planesInWorldSpace(std::vector<Plane>&) const;
};

void applyTransToGameobj(const Transform& t, GameObj& go);
void applyTransToGameobjLocally(const Transform& t, GameObj& go);


} // end namespace