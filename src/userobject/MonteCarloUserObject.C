
#include "MonteCarloUserObject.h"
#include "LineSegment.h"
#include "libmesh/plane.h"

template<>
InputParameters validParams<MonteCarloUserObject>()
{
	InputParameters params = validParams<SideUserObject>();
	return params;
}

MonteCarloUserObject::MonteCarloUserObject(const InputParameters & parameters) :
		SideUserObject(parameters)
{
}

void MonteCarloUserObject::execute()
{
	LineSegment line_segment(Point(1, 0, 1), Point(0, 0, -1));
	Plane plane(Point(0, 0, 0), Point(0, 1, 1));

	Point p;

	std::cout << _current_elem->() <<std::endl;

//	if(line_segment.intersect(plane, p))
//		std::cout << p << std::endl;
//	std::cout << "test "  << std::endl;
}
