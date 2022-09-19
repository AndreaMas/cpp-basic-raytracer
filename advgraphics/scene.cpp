#include "scene.h"
#include "camera.h"
#include "transform.h"
#include "shapes3d.h"
#include <iostream>
#include <string>
#include <vector>

using namespace mgd;

// Turns vector of GameObjects into vector of Spheres, which can be rendered
void Scene::toWorld(std::vector<Sphere>& allSpheres) const
{
	allSpheres.clear(); // clears old sphereList
	for (const GameObj &go : allGameObjs) {
		allSpheres.push_back(applyTransToSphere(go.transform, go.nose));
		allSpheres.push_back(applyTransToSphere(go.transform, go.body));
	}
}

void Scene::populate(int numGameobjs)
{
	for (int i = 0; i < numGameobjs; i++) {
		GameObj newGameobj;
		newGameobj.transform.position = Vector3::randomVector(-2,2) + Vector3(0,0,10);
		//newGameobj.transform.rotation = ;
		//newGameobj.transform.scale = ;
		allGameObjs.push_back(newGameobj);
	}
}

void Scene::decimate()
{
	allGameObjs.clear();
}

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
	now += 0.005;
	return now;
	// return std::chrono::system_clock().now();
}


// scene from lesson 1
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
