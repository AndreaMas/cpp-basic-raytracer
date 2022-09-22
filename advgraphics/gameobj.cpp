#include "gameobj.h"

using namespace mgd;

GameObj::GameObj():
	transform(), //identity
	forward(Vector3(1,0,0)),
	body(Vector3(0, 0, 0), 1),
	nose(Vector3(0, 0, 0.8), 0.5)
{

}

void mgd::applyTransToGameobj(const Transform& t, GameObj& go)
{
	go.forward = t.transformVersor(go.forward);
	go.transform = t * go.transform;
}

void mgd::applyTransToGameobjLocally(const Transform& t, GameObj& go)
{
	go.forward = t.transformVersor(go.forward); // maybe wrong ...
	go.transform = go.transform * t;
}

Sphere mgd::GameObj::noseInWorldSpace() const
{
	return applyTransToSphere(transform, nose);
}

Sphere mgd::GameObj::bodyInWorldSpace() const
{
	return applyTransToSphere(transform, body);
}
