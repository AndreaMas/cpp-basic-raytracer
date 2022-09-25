#include "camera.h"

using namespace mgd;

Camera::Camera() :focal(1.0f), pixelDimX(5), pixelDimY(5) {}

Camera::Camera(Scalar _focal, int _pixelDimX, int _pixelDimY)
	:focal(_focal), pixelDimX(_pixelDimX), pixelDimY(_pixelDimX) {}

Ray Camera::primaryRay(int pixelX, int pixelY) {
	Ray ray(Point3(0,0,0),Vector3());
	Scalar clipX = 2.0f * pixelX / pixelDimX - 1.0;
	Scalar clipY = -2.0f * pixelY / pixelDimY + 1.0;
	ray.d = Vector3(clipX, clipY, focal).normalized();
	return ray;
}

int Camera::GetPixelDimX() { return pixelDimX; }

int Camera::GetPixelDimY() { return pixelDimY; }
