
#pragma once

#include "RayLine.h"
#include "libmesh/elem.h"
#include <map>
using std::map;

class UserDefinedElem;

class SideElement
{

public:
	SideElement(const Elem *elem, const Point normal, Real absorptivity = 0.5, Real diffuse_reflectivity = 0.5, Real mirrors_reflectivity = 0.5);

public:
	RayLine sendRay();
	RayLine diffuseReflectRay(RayLine * rayline, Point point);
	RayLine mirrorsReflectRay(RayLine * rayline, Point point);
	Real _absorptivity;
	Real _diffuse_reflectivity;
	Real _mirrors_reflectivity;
	const Elem * _elem;

	map<SideElement*, Real> _rd;
	UserDefinedElem* _belong_to_which_elem;
	int _is_which_sideelem;

protected:
//	const Elem * _elem;
	Point _normal;
};
