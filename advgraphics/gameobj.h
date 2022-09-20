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
	Sphere noseInWorldSpace() const;
	Sphere bodyInWorldSpace() const;
	
};

//void applyTransToGameobj(const Transform& t, const GameObj& go);
void applyTransToGameobj(const Transform& t, GameObj& go);
// void applyTransToSphereRef(const Transform& a, Sphere& s);
} // end namespace