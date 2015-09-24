
#include "RayLine.h"

RayLine::RayLine() :
  LineSegment(Point(0), Point(1)),
  _lenght(0)
{

}

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
