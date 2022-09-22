#pragma once
#include <vector>
#include "gameobj.h"
#include "transform.h"
#include "shapes3d.h"



namespace mgd {

class FaceObj : public GameObj {

public:
	Sphere body, nose;

	FaceObj();
	std::vector<Sphere> meshInWorldSpace() const;

};

} // end namespace