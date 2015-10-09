
#pragma once

#include "LineSegment.h"
#include "libmesh/elem.h"
#include "libmesh/plane.h"
#include <vector>
using std::vector;

class RayLine : public LineSegment
{
public:
	RayLine();
	RayLine(const Point &p0, const Point &p1, Real lenght);
	RayLine(const Point &p0, const Point &normal);
	bool sideIntersectedByLine(const Elem* side, Point& point);
	Point _normal;
	Point _p0;
	Point _p1;
//	Point & start(){return _p0;}
	Real _lenght;
	int _count;
private:
//	vector<SideElement*> path;

};
