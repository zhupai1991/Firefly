
#include "RayLine.h"

RayLine::RayLine() :
  LineSegment(Point(0), Point(1)),
  _lenght(0)
{

}

RayLine::RayLine(const Point &p0, const Point &p1, Real lenght) :
	LineSegment(p0, p1),
	_normal( (p1-p0).unit() ),
	_p0(p0),
	_p1(p1),
	_lenght(lenght)
{
}

RayLine::RayLine(const Point &p0, const Point &normal):
	LineSegment(p0, p0+normal*100000),
	_normal(normal),
	_p0(p0),
	_p1(p0+normal*100000),
	_lenght(100000)
{
}

bool RayLine::sideIntersectedByLine(const Elem* side, Point& point)
{
	bool inter(false);
	unsigned int dim = side->dim();
	if (dim == 3)
	{
		Plane plane(*side->get_node(0), *side->get_node(1), *side->get_node(2));
		inter = intersect(plane, point);
	}
	else if (dim == 2)
	{
		RayLine side_ray(*side->get_node(0), *side->get_node(1));
		inter = intersect(side_ray, point);
	}

	else // 1D
	{
		inter = contains_point(*side->get_node(0));
		if (inter)
		point = *side->get_node(0);
	}

	if(side->contains_point(point))
		return inter;

	else
		return false;
}
