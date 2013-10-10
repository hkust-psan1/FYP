#ifndef _SCENE_H
#define _SCENE_H

#include "object.h"
#include "intersection.h"
#include "ray.h"

class Light;

class Scene 
{
public:
	Scene(std::vector<Object*> obj);
	Intersection* intersect(const Ray* r);
	inline std::vector<Object*> getObjects() const { return objects; };
	
	static Vec3 ambient;
	static void setAmbient(Vec3 a) { ambient = a; }
	void buildBox();
	BoundingBox* root;

	std::vector<Object*> objects;
    std::vector<Light*> lights;
};

#endif