#include "scene.h"
#include "camera.h"
#include "transform.h"
#include "shapes3d.h"
#include "gameobj.h"
#include <iostream>
#include <string>
#include <vector>

using namespace mgd;


// bring spheres & planes of gameobjects in world space
void Scene::toWorld(std::vector<Sphere>& allSpheres, std::vector<Plane>& allPlanes)
{
	allSpheres.clear();
	allPlanes.clear();

	for (const GameObj& go : gameObjs) {
		go.spheresInWorldSpace(allSpheres);
		go.planesInWorldSpace(allPlanes);
	}
}


void Scene::populate(int numGameObj)
{
	// add plane gameobject
	Plane plane(Point3(0, -2, 0), Versor3(0, 1, 0));
	GameObj po;
	po.planes.push_back(plane);
	gameObjs.push_back(po);

	// add "faces" gameobjects
	for (int i = 0; i < numGameObj; i++) {
		GameObj go;
		go.transform.position = Vector3::randomVector(-10.0f,10.0f) + Vector3(0,0,5);
		go.transform.rotation = Quaternion::fromAngleAxis(180, Vector3::randomVector(0.0f, 1.0f));

		Sphere body(Vector3(0,0,0), 1);
		Sphere nose(Vector3(0,0,0.8f), 0.5f);
		go.spheres.push_back(body);
		go.spheres.push_back(nose);

		gameObjs.push_back(go);
	}

}


void Scene::decimate()
{
	gameObjs.clear();
}

void Scene::transformAll(const Transform& t)
{
	// transform gameobjects
	for (unsigned int i = 0; i < gameObjs.size(); i++) {
		applyTransToGameobj(t, gameObjs.at(i));
	}
}

void Scene::transformAllLocally(const Transform& t)
{
	for (unsigned int i = 0; i < gameObjs.size(); i++) {
		applyTransToGameobjLocally(t, gameObjs.at(i));
	}
}

void Scene::transformJust(int index, const Transform& t)
{
	if (index < 0 || index > gameObjs.size()) {
		std::cout << "Index out of bounds ..." << std::endl;
		return;
	} 
	applyTransToGameobjLocally(t, gameObjs.at(index));
}

void Scene::toView(int index)
{
	if (index < 0 || index > gameObjs.size()) {
		std::cout << "Index out of bounds ..." << std::endl;
		return;
	}
	transformAll(gameObjs.at(index).transform);
}

/*
	====================================================
	Raycast Related
	====================================================
*/

const char* intensityToCstr(Scalar intensity) {
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


const char* lighting(Versor3 normal, const Versor3& lightDir) {
	// Lambert lighting
	Scalar diffuse = dot(normal, lightDir);
	if (diffuse < 0) diffuse = 0;
	return intensityToCstr(diffuse);
}


float mgd::currentTime() {
	static float now = 0.0f;
	now += 0.005f;
	return now;
	// return std::chrono::system_clock().now();
}


void mgd::rayCasting(const std::vector<Sphere>& sphereVector, const std::vector<Plane>& planeVector) {
	//float time = currentTime();
	Camera cam(2.0f, 45, 45);
	Versor3 lightDir = Versor3(1, 2, -2).normalized();

	std::string screenBuffer; // string to get ready and print all at once

	for (int y = 0; y < cam.GetPixelDimX(); y++) {
		for (int x = 0; x < cam.GetPixelDimX(); x++) {
			Point3 hitPos;
			Versor3 hitNorm;
			Scalar distMax = 1000.0f;

			for (Plane p : planeVector) {
				rayCast(cam.primaryRay(x, y), p, hitPos, hitNorm, distMax);
			}

			for (Sphere s : sphereVector) {
				rayCast(cam.primaryRay(x, y), s, hitPos, hitNorm, distMax);
			}

			screenBuffer += lighting(hitNorm, lightDir);
		}
		screenBuffer += "\n";
	}
	std::cout << screenBuffer;
}
