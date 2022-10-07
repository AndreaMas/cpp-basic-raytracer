#include "gameobj.h"

using namespace mgd;

GameObj::GameObj():
	transform() //identity
{}

void GameObj::spheresInWorldSpace(std::vector<Sphere>& allSpheres) const
{
	for (const Sphere s : spheres) {
		allSpheres.push_back(applyTransToSphere(transform, s));
	}
}

void GameObj::planesInWorldSpace(std::vector<Plane>& allPlanes) const
{
	for (const Plane p : planes) {
		allPlanes.push_back(applyTransToPlane(transform, p));
	}
}

void mgd::applyTransToGameobj(const Transform& t, GameObj& go)
{
	go.transform = t * go.transform;
}

void mgd::applyTransToGameobjLocally(const Transform& t, GameObj& go)
{
	go.transform = go.transform * t;
}


