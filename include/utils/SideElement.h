
#pragma once

#include "RayLine.h"
#include "libmesh/elem.h"

class SideElement
{

public:
	SideElement(const Elem * elem, const Point normal);

public:
	RayLine SendRay();
	RayLine DiffuseReflectRay(const RayLine * rayline, Point point);
	RayLine MirrorsReflectRay(const RayLine * rayline, Point point);
	Real Absorptivity;
	Real Diffuse_Reflectivity;
	Real Mirrors_Reflectivity;
	Real RD();
	int MaxReflectCount;


protected:
	const Elem * _elem;
	Point _normal;
};
