#pragma once
#include "vector3.h"
#include "shapes3d.h"

namespace mgd {


class Camera {
private:
	Scalar focal;
	int pixelDimX, pixelDimY;
public:
	Camera();
	Camera(Scalar _focal, int _pixelDimX, int _pixelDimY);
	Ray primaryRay(int pixelX, int pixelY);
	int GetPixelDimX();
	int GetPixelDimY();
};


} // end mgd namespace


