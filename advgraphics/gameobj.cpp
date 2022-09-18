#include "gameobj.h"

using namespace mgd;

GameObj::GameObj():
	transform(), //identity
	body(Vector3(0, 0, 0), 1),
	nose(Vector3(0.8, 0, 0), 0.5)
{

}
