#pragma once
#include <vector>
#include "shapes3d.h"
#include "faceObj.h"
#include "planeObj.h"
#include "gameobj.h"


namespace mgd {

class Scene {
public:
	std::vector<GameObj> gameObjs; // a vector of gameobjects, each with its own transform
	std::vector<FaceObj> faceObjs; // a vector of gameobjects, each with its own transform
	std::vector<PlaneObj> planeObjs; // a vector of gameobjects, each with its own transform

	void populateGameObjs(int numGameObj);
	void populateFaceObjs(int numFaceObj); // populate allGameObjs with new randomly placed gameobjects
	void populatePlane();
	void decimate(); // destroy all allGameObjs
	void GameObjsToWorld(std::vector<Sphere>& allSpheres) const; // produces a vector of spheres in world space
	void FaceObjsToWorld(std::vector<Sphere>& allSpheres) const; // produces a vector of spheres in world space
	void PlaneObjsToWorld(std::vector<Plane>& allPlanes) const; // produces a vector of planes in world space
	void transformAll(const Transform&);
	void transformAllLocally(const Transform&);
	void transformJust(int, const Transform&);
	
};

// Raycast related
float currentTime();
void rayCasting(const std::vector<Sphere>& sphereVector, const std::vector<Plane>& planeVector);

} // end namespace mgd

