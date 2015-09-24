
#pragma once

#include "LineSegment.h"
#include <vector>
using std::vector;

class RayLine : public LineSegment
{
public:
	RayLine();
	RayLine(const Point &p0, const Point &p1, Real lenght);
	RayLine(const Point &p0, const Point &normal);
	Point normal;
	Point p0;
//	Point & start(){return _p0;}
private:
	Real _lenght;
//	vector<SideElement*> path;

};
