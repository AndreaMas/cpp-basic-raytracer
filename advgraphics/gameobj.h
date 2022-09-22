#pragma once
#include "transform.h"
#include "shapes3d.h"



namespace mgd {

class GameObj {

public:
	Transform transform;
	Versor3 forward;
	// hereunder mesh, collider, ...,  all in local space
	Sphere body, nose;
	
	GameObj();
	Sphere noseInWorldSpace() const; // TODO: list of all spheres should be returned
	Sphere bodyInWorldSpace() const;
	
};

void applyTransToGameobj(const Transform& t, GameObj& go);
void applyTransToGameobjLocally(const Transform& t, GameObj& go);

} // end namespace