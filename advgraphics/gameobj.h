#pragma once
#include "transform.h"
#include "shapes3d.h"
#include <vector>



namespace mgd {

class GameObj {

public:
	Transform transform;
	
	GameObj();
	virtual std::vector<Sphere> meshInWorldSpace() const;
};

void applyTransToGameobj(const Transform& t, GameObj& go);
void applyTransToGameobjLocally(const Transform& t, GameObj& go);


} // end namespace