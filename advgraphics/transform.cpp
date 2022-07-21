#include "transform.h"

using namespace mgd;

Transform::Transform():rotate(Quaternion::identity()), 
						scale(1.0f), translate(0,0,0) {}

Vector3 Transform::transformPoint(const Vector3& p)
{
	return rotate.applyRotationTo(p * scale) + translate;
}

Vector3 Transform::transformVector(const Vector3& p)
{
	return rotate.applyRotationTo(p * scale);
}

Vector3 Transform::transformVersor(const Vector3& p)
{
	return rotate.applyRotationTo(p);
}

Transform Transform::inverse() const
{
	Transform t;
	t.scale = (1 / scale);
	t.rotate = rotate.conjugated();
	t.translate = t.rotate.applyRotationTo(-translate * t.scale);
	return t;
}

void Transform::invert()
{
	scale = (1 / scale);
	rotate.conjugate();
	translate = rotate.applyRotationTo(-translate * scale);
}




