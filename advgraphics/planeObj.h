#pragma once
#include <vector>
#include "gameobj.h"
#include "transform.h"
#include "shapes3d.h"



namespace mgd {

	class PlaneObj : public GameObj {

	public:
		Plane plane;

		PlaneObj();
		PlaneObj(Point3 p, Versor3 n);
		Plane planeMeshInWorldSpace() const;

	};

} // end namespace