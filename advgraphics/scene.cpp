#include "scene.h"
#include "camera.h"
#include "transform.h"
#include "shapes3d.h"
#include "gameobj.h"
#include "faceObj.h"
#include <iostream>
#include <string>
#include <vector>

using namespace mgd;

// Turns vector of GameObjects into vector of Spheres, which can be rendered
void Scene::toWorld(std::vector<Sphere>& allSpheres) const
{
	allSpheres.clear(); // clears old sphereList
	for (const GameObj& go : allGameObjs) {
		std::vector<Sphere> goMesh = go.meshInWorldSpace();
		allSpheres.insert(goMesh.begin(), allSpheres.begin(), allSpheres.end());
	}
}

void Scene::populate(int numGameobjs)
{
	for (int i = 0; i < numGameobjs; i++) {
		FaceObj newGameobj;
		newGameobj.transform.position = Vector3::randomVector(-10.1f,10.1f) + Vector3(0,0,5);
		newGameobj.transform.rotation = Quaternion::fromAngleAxis(180, newGameobj.transform.position);
		newGameobj.transform.position.y = 0;
		
		//newGameobj.transform.scale = ;
		allGameObjs.push_back(newGameobj);
	}
}

void Scene::decimate()
{
	allGameObjs.clear();
}

void Scene::transformAll(const Transform& t)
{
	for (unsigned int i = 0; i < allGameObjs.size(); i++) {
		applyTransToGameobj(t, allGameObjs.at(i));
	}
}

void Scene::transformAllLocally(const Transform& t)
{
	for (unsigned int i = 0; i < allGameObjs.size(); i++) {
		applyTransToGameobjLocally(t, allGameObjs.at(i));
	}
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


void mgd::rayCasting(const std::vector<Sphere>& sphereVector) {
	//float time = currentTime();
	Camera cam(2.0f, 45, 45);
	Versor3 lightDir = Versor3(1, 2, -2).normalized();
	//Plane plane1(Point3(0, -1, 0), Versor3(0, 1, 0));

	std::string screenBuffer; // string to get ready and print all at once

	for (int y = 0; y < cam.GetPixelDimX(); y++) {
		for (int x = 0; x < cam.GetPixelDimX(); x++) {
			Point3 hitPos;
			Versor3 hitNorm;
			Scalar distMax = 1000.0f;

			for (Sphere s : sphereVector) {
				rayCast(cam.primaryRay(x, y), s, hitPos, hitNorm, distMax);
			}
						
			//rayCast(cam.primaryRay(x, y), plane1, hitPos, hitNorm, distMax);

			screenBuffer += lighting(hitNorm, lightDir);
		}
		screenBuffer += "\n";
	}
	std::cout << screenBuffer;
}
