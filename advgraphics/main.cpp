#include <iostream>
#include <assert.h>
#include <conio.h>
//#include <chrono>

#include "vector3.h"
#include "quaternion.h"

#include "shapes3d.h"
#include "camera.h"

#include "transform.h"
#include "gameobj.h"
#include "scene.h"

using namespace mgd;

constexpr double PI_DOUBLE = 3.14159265358979;

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
	Modes: user changes what to do by pressing 'm' key
	====================================================
*/

namespace mgd {
	enum Modes {
		MOVEMENT = 0,
		MOVE_OBJS = 1,
		MOVE_OBJ = 2,
		POSSESS = 3,
		Last = 4 // just for iteration
	};
}

/*
	====================================================
	Utilities
	====================================================
*/
#pragma region Utilities
int InputAsciiValue() {
	// Check keys pressed
	char keyPress = _getch();
	return (int)keyPress;
}

void InputCheckCharToAsciiValue() {
	char key_press;
	int ascii_value;
	std::cout << "\n\t\t\tPress Any Key To Check  Its ASCI Value\n\n\t\t\tPress ESC to EXIT\n\n\n";
	while (1)
	{
		key_press = _getch();
		ascii_value = key_press;
		if (ascii_value == 27) // For ESC
			break;
		std::cout << "\t\t\tKEY Pressed-> \" " << key_press << " \" Ascii Value =  " << ascii_value << "\n\n";
	}
}

Modes NextMode(Modes mode) {
	mode = Modes(mode + 1); // iterate through different modes
	if (mode == Last) mode = Modes(0);
	return mode;
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

	// World Axis
	Vector3 wX(1, 0, 0); // cam right
	Vector3 wY(0, 1, 0); // cam up
	Vector3 wZ(0, 0, 1); // cam forward

	// Movement & rotation magnitude
	const float stepVal = 0.1f;
	const float turnDegrees = 0.3f;
	const float turnRad = turnDegrees * (float)PI_DOUBLE / 180.0f;

	// Movement & rotation vectors
	Vector3 forward   (0, 0,  stepVal);
	Vector3 backwards (0, 0, -stepVal);
	Vector3 left      (-stepVal, 0, 0);
	Vector3 right     ( stepVal, 0, 0);
	Quaternion yawnLeft  (wY, turnRad);
	Quaternion yawnRight (wY, -turnRad);

	// Setup scene 
	Scene s;
	std::vector<Sphere> allSpheres;
	std::vector<Plane> allPlanes;

	// Populate scene
	int numGameObjects = 20;
	int numFaceObjects = 20;
	s.populate(numGameObjects);

	// First render
	s.toWorld(allSpheres, allPlanes);
	mgd::rayCasting(allSpheres, allPlanes);

	// initial Mode
	std::cout << "Mode MOVEMENT: Move in world with WASD and JL to turn around." << std::endl;
	Modes mode = MOVEMENT;

	// other initializers
	int idGameObj = 0;

	while (1) {
		
		// check Input ascii values
		//InputCheckCharToAsciiValue();

		// Check keys pressed
		char key_press = _getch();
		int ascii_value = (int)key_press;

		// Default transform to apply
		Transform t;

		// Change Transform or Mode depending from user input
		switch (ascii_value) {
		case 27: // ESC
			return 0;
		case 119: // W
			t.position = forward;
			break;
		case 97:  // A
			t.position = left;
			break;
		case 115: // S 
			t.position = backwards;
			break;
		case 100: // D
			t.position = right;
			break;
		case 105: // I
			t.rotation = Quaternion();
			break;
		case 106: // J
			t.rotation = yawnLeft;
			break;
		case 107: // K
			t.rotation = Quaternion();
			break;
		case 108: // L
			t.rotation = yawnRight;
			break;
		case 109: // M (mode)
			mode = MOVEMENT;
			std::cout << " ----> MODE MOVEMENT: Move in world with WASD and JL to turn around. <--- " << std::endl;
			break;
		case 110: // N (mode)
			mode = MOVE_OBJS;
			std::cout << " ----> MODE MOVE_OBJS: Move and rotate all gameobjects (WASD and JL). <--- " << std::endl;
			break;
		case 111: // O (mode)
			mode = MOVE_OBJ;
			std::cout << " ----> MODE MOVE_OBJ: Move and rotate 1 gameobject (WASD and JL). <--- " << std::endl;
			std::cout << " ----> Select gameobject with numbers 1-9: " << std::endl;
			idGameObj = InputAsciiValue() - 48;
			break;
		case 112: // P (mode)
			mode = POSSESS;
			std::cout << " ----> MODE POSSESS: inherit a gameobjects position and rotation.<--- " << std::endl;
			std::cout << " ----> Select gameobject with numbers 1-9: " << std::endl;
			idGameObj = InputAsciiValue() - 48;
			break;
		default: break;
		}


		// Apply transform
		t.invert();
		switch (mode) {
		case MOVEMENT:
			s.transformAll(t);
			s.transformAll(t);
			break;
		case MOVE_OBJS:
			s.transformAllLocally(t);
			s.transformAllLocally(t);
			break;
		case MOVE_OBJ:
			s.transformJust(idGameObj,t);
			s.transformJust(idGameObj,t);
			break;
		case POSSESS:
			s.toView(idGameObj);
			mode = MOVEMENT;
			break;
		default: break;
		}

		// Render
		s.toWorld(allSpheres, allPlanes);
		mgd::rayCasting(allSpheres, allPlanes);

	}

}


