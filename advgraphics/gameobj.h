#pragma once
#include "transform.h"
#include "shapes3d.h"

namespace mgd {

class GameObj {

public:
	Transform transform;
	// hereunder mesh, collider, ...,  all in local space
	Vector3 forward;
	Sphere body, nose;
	
	GameObj();
};

}

