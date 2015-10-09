
#pragma once

#include "RayLine.h"
#include "libmesh/elem.h"
#include <map>
using std::map;

class SideElement
{

public:
	SideElement(const Elem * elem, const Point normal);

public:
	RayLine SendRay();
	RayLine DiffuseReflectRay(RayLine * rayline, Point point);
	RayLine MirrorsReflectRay(RayLine * rayline, Point point);
	Real Absorptivity;
	Real Diffuse_Reflectivity;
	Real Mirrors_Reflectivity;
	Real RD();
	int MaxReflectCount;
	const Elem * _elem;

	map<SideElement*, Real> _rd;

protected:
//	const Elem * _elem;
	Point _normal;
};
