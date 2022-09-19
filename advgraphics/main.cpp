#include <iostream>
#include <assert.h>
//#include <chrono>

#include "vector3.h"
#include "shapes3d.h"
#include "camera.h"
#include "quaternion.h"
#include "transform.h"

#include "scene.h"



using namespace mgd;

/*
	====================================================
	Unit tests
	====================================================
*/
#pragma region UnitTests
void unitTestLinearOps() {
	Vector3 a(24, 1, 32), b(12,3,54);
	Scalar k = 3.2f;
	assert(areEqual(a+b,b+a));
	assert(areEqual(a*k + b*k,(a+b)* k));
}
void unitTestProducts() {
	Vector3 a(22, -3, 10), b(12, 11, -20), c(10, 22, -45);
	Scalar k = 0.4f;
	assert(isZero(dot(a, cross(a, b))));
	assert(isZero(dot(b, cross(a, b))));
	assert(isZero(cross(a,b) + cross(b,a)));
}
void UnitTestRaycast() {
	Ray r(Point3(0,0,0), Versor3(1,0,0));
	Sphere s(Point3(5,0,0), 3);
	Point3 hitPoint;
	Versor3 hitNorm;
	Scalar distMax = 1000.0f;
	bool isHit = rayCast(r, s, hitPoint, hitNorm, distMax);
	assert(isHit);
	assert(areEqual(hitPoint, Point3(2, 0, 0)));
}
void UnitTestRaycastPlane() {
	// ...
}
void unitTestQuaternions() {
	{ // test 180deg rotation on y axis
		Quaternion rot = Quaternion::fromAngleAxis(180, Vector3(0, 1, 0));
		Vector3 p(0, 0, 1);
		Vector3 test = rot.applyRotationTo(p);
		assert(areEqual(rot.applyRotationTo(p), Vector3(0, 0, -1)));
	}
	{ // test 90deg rotation on y axis
		Quaternion rot = Quaternion::fromAngleAxis(90, Vector3(0, 1, 0));
		Vector3 p(3, 5, 6);
		Vector3 test = rot.applyRotationTo(p);
		assert(areEqual(rot.applyRotationTo(p), Vector3(6, 5, -3)));
	}
	{ // test twelve 30deg rotation on some axis
		Quaternion rot = Quaternion::fromAngleAxis(30, Vector3(1, 1, 3));
		Vector3 p(3, 5, 6), q = p;
		for (int k = 0; k < 12; k++) q = rot.applyRotationTo(q);
		assert(areEqual(q, p));
	}
	{ // cumulating 4 and 8 times identity(0001) rotation
		Quaternion q = Quaternion::identity();
		Quaternion qRot = Quaternion::fromAngleAxis(90.0f, Vector3(3, 5, 6));
		for (int k = 0; k < 4; k++) q = q * qRot;
		assert(areEquivalent(q, Quaternion::identity()));
		for (int k = 0; k < 4; k++) q = q * qRot;
		assert(areEqual(q, Quaternion::identity()));
	}
}
void unitTestTransformations() {
	{ // test t.inverse() and t.invert()
		Transform t;
		t.rotation = Quaternion::fromAngleAxis(22, Vector3(0, 1, 0));
		t.position = Vector3(30, 4, 1);
		t.scale = 2;
		Point3 p(-31, 12, 1);
		Point3 q = t.transformPoint(p);
		Point3 r = t.inverse().transformPoint(q);
		assert(areEqual(p, r));

		Transform t2 = t;
		Point3 p2(30, -12, 11);
		Point3 q2 = t2.transformPoint(p2);
		t2.invert();
		Point3 r2 = t2.transformPoint(q2);
		assert(areEqual(p2, r2));
	} 
	{ // test transform cumulation through * operator
		Transform tA;
		tA.rotation = Quaternion::fromAngleAxis(22, Vector3(0, 1, 0));
		tA.position = Vector3(3, 4, 1);
		tA.scale = 2.0f;
		Transform tB;
		tB.rotation = Quaternion::fromAngleAxis(33, Vector3(2, 1, 3));
		tB.position = Vector3(1, 2, 3);
		tB.scale = 1.2f;
		Transform tAB = tA * tB;
		Point3 p(-31, 12, 1);
		assert(areEqual(
			tAB.transformPoint(p), 
			tA.transformPoint(tB.transformPoint(p))
		));
	}

}
#pragma endregion


/*
	====================================================
	Lesson 1 Old Scene
	====================================================
*/
#pragma region Lesson1
float currentTimeLesson1() {
	static float now = 0.0f;
	now += 0.005;
	return now;
	// return std::chrono::system_clock().now();
}

const char* intensityToCstrLesson1(Scalar intensity) {
	// convert intensity to ashii art value
	switch (int(round(intensity * 8))) {
	case 0: return "  "; // darkest
	case 1: return " '";
	case 2: return " +";
	case 3: return " *";
	case 4: return " #";
	case 5: return "'#";
	case 6: return "+#";
	case 7: return "*#";
	case 8: return "##"; // lightest
	default: return "??";
	}
}

const char* lightingLesson1(Versor3 normal, const Versor3& lightDir) {
	// Lambert lighting
	Scalar diffuse = dot(normal, lightDir);
	if (diffuse < 0) diffuse = 0;
	return intensityToCstrLesson1(diffuse);
}


void rayCastingSphereLesson1() {  // scene from lesson 1
	float time = currentTimeLesson1();
	Camera cam(2.0f,45,45);
	Versor3 lightDir = Versor3(1, 2, -2).normalized();
	lightDir = Versor3(1,2,3*std::cos(time*0.2)).normalized();
	Sphere sphere1(Point3(0, 0, 6), 2);
	Sphere sphere2(Point3(2.0 * std::cos(time), 1, 6 + 2.0 * std::sin(time)), 1);
	Plane plane1(Point3(0,-1,0), Versor3(0,1,0));

	std::string screenBuffer; // string to get ready and print all at once

	for (int y = 0; y < cam.GetPixelDimX(); y++) {
		for (int x = 0; x < cam.GetPixelDimX(); x++) {
			Point3 hitPos;
			Versor3 hitNorm;
			Scalar distMax = 1000.0f;

			rayCast(cam.primaryRay(x, y), sphere1, hitPos, hitNorm, distMax);
			rayCast(cam.primaryRay(x, y), plane1, hitPos, hitNorm, distMax);
			rayCast(cam.primaryRay(x, y), sphere2, hitPos, hitNorm, distMax);
			screenBuffer += lightingLesson1(hitNorm, lightDir);
		}
		screenBuffer += "\n";
	}
	std::cout << screenBuffer;
}
#pragma endregion


/*
	====================================================
	Main
	====================================================
*/

int main() {


	// unit tests
	{
	unitTestLinearOps();
	unitTestProducts();
	UnitTestRaycast();
	UnitTestRaycastPlane();
	unitTestQuaternions();
	unitTestTransformations();
	}

	Scene s;
	int num_gameobjects = 50;
	s.populate(num_gameobjects);
	std::vector<Sphere> allSpheres;
	s.toWorld(allSpheres);

	rayCasting(allSpheres);
	
	while (1) {
		//if (((int)currentTime() % 10)) break;
		// working scene from lesson 1
		// rayCastingSphereLesson1();
		s.decimate();
		s.populate(3);
		s.toWorld(allSpheres);
		rayCasting(allSpheres);
	}
		

	
	



}


