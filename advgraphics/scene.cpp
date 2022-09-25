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
void Scene::GameObjsToWorld(std::vector<Sphere>& allSpheres) const
{
	allSpheres.clear(); // clears old sphereList
	for (const GameObj& go : gameObjs) {
		std::vector<Sphere> goSpheres = go.meshInWorldSpace();
		for (const Sphere& sphere : goSpheres) {
			allSpheres.push_back(sphere);
		}
	}
}

// Turns vector of FaceObjects into vector of Spheres, which can be rendered
void Scene::FaceObjsToWorld(std::vector<Sphere>& allSpheres) const
{
	allSpheres.clear(); // clears old sphereList
	for (const FaceObj& fo : faceObjs) {
		std::vector<Sphere> goSpheres = fo.meshInWorldSpace();
		for (const Sphere& sphere : goSpheres) {
			allSpheres.push_back(sphere);
		}
	}
}

// Turns vector of PlaneObjects into vector of Planes, which can be rendered
void Scene::PlaneObjsToWorld(std::vector<Plane>& allPlanes) const
{
	allPlanes.clear(); // clears old planeList

	for (const PlaneObj& po : planeObjs) {
		allPlanes.push_back(po.planeMeshInWorldSpace());
	}
}

void Scene::populateGameObjs(int numGameObj)
{
	// spawn plain gameobjects (no mesh or anything, only transform)
	for (int i = 0; i < numGameObj; i++) {
		GameObj go;
		go.transform.position = Vector3::randomVector(-10.0f,10.0f) + Vector3(0,0,5);
		go.transform.rotation = Quaternion::fromAngleAxis(180, Vector3::randomVector(0.0f, 1.0f));
		//newGameobj.transform.scale = ;

		gameObjs.push_back(go);
	}

}

void Scene::populateFaceObjs(int numFaceObj)
{
	// spawn face objects
	for (int i = 0; i < numFaceObj; i++) {
		FaceObj fo;
		fo.transform.position = Vector3::randomVector(-10.0f, 10.0f) + Vector3(0, 0, 5);
		fo.transform.rotation = Quaternion::fromAngleAxis(180, Vector3::randomVector(0.0f, 1.0f));
		//newGameobj.transform.scale = ;
		fo.transform.position.y = 0;

		faceObjs.push_back(fo);
	}
}


void Scene::populatePlane()
{
	PlaneObj planeObj(Point3(0,-1,0), Versor3(0,1,0));
	planeObjs.push_back(planeObj);
}

void Scene::decimate()
{
	gameObjs.clear();
	faceObjs.clear();
	planeObjs.clear();
}

void Scene::transformAll(const Transform& t)
{
	// transform gameobjects
	for (unsigned int i = 0; i < gameObjs.size(); i++) {
		applyTransToGameobj(t, gameObjs.at(i));
	}

	// transform faceobjects
	for (unsigned int i = 0; i < faceObjs.size(); i++) {
		applyTransToGameobj(t, faceObjs.at(i));
	}

	// transform planes
	for (unsigned int i = 0; i < planeObjs.size(); i++) {
		applyTransToGameobj(t, planeObjs.at(i));
	}
}

void Scene::transformAllLocally(const Transform& t)
{
	for (unsigned int i = 0; i < gameObjs.size(); i++) {
		applyTransToGameobjLocally(t, gameObjs.at(i));
	}

	// transform faceobjects
	for (unsigned int i = 0; i < faceObjs.size(); i++) {
		applyTransToGameobjLocally(t, faceObjs.at(i));
	}

	// transform planes
	for (unsigned int i = 0; i < planeObjs.size(); i++) {
		applyTransToGameobjLocally(t, planeObjs.at(i));
	}
}

void Scene::transformJust(int index, const Transform& t)
{
	applyTransToGameobjLocally(t, faceObjs.at(index));
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
