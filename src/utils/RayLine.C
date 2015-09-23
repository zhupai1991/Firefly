
#include "RayLine.h"

RayLine::RayLine(const Point &p0, const Point &p1, Real lenght) :
	LineSegment(p0, p1),
	_lenght(lenght)
{
}

RayLine::RayLine(const Point &p0, const Point &normal):
	LineSegment(p0, p0+normal*100000),
	_lenght(100000)
{
}
