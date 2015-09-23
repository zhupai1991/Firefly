
#pragma once

#include "LineSegment.h"

class RayLine : public LineSegment
{
public:
	RayLine(const Point &p0, const Point &p1, Real lenght);
	RayLine(const Point &p0, const Point &normal);

//	Point & start(){return _p0;}
private:
	Real _lenght;
};
