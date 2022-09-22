#include "gameobj.h"

using namespace mgd;

GameObj::GameObj():
	transform() //identity
{

}

std::vector<mgd::Sphere> GameObj::meshInWorldSpace() const
{
	std::vector<Sphere> v;
	return v;
}

void mgd::applyTransToGameobj(const Transform& t, GameObj& go)
{
	go.transform = t * go.transform;
}

void mgd::applyTransToGameobjLocally(const Transform& t, GameObj& go)
{
	go.transform = go.transform * t;
}


