
#include "MonteCarloUserObject.h"
#include "FEProblem.h"
#include "libmesh/elem.h"
#include "RayLine.h"
#include "MooseRandom.h"

#include "libmesh/plane.h"
#include "SideElement.h"


template<>
InputParameters validParams<MonteCarloUserObject>()
{
	InputParameters params = validParams<SideUserObject>();
	params += validParams<RandomInterface>();
	return params;
}

MonteCarloUserObject::MonteCarloUserObject(const InputParameters & parameters) :
				SideUserObject(parameters),
				RandomInterface(parameters, *parameters.get<FEProblem *>("_fe_problem"), parameters.get<THREAD_ID>("_tid"), false)

{
}

void MonteCarloUserObject::execute()
{
	SideElement side_element(_current_elem, _normals[0]);



	std::cout << side_element.SendRay().p0;
//	int particle_count=1000000;
//	vector<const Elem*> elem_vec;
//	int n_elem=elem_vec.size();
//	float RD[n_elem][n_elem];
//
//	for(int i=0;i<particle_count;i++)

	vector<const Elem*> elem_vec;
	elem_vec.push_back(_current_elem);

	RayLine ray_line(Point(1, 0, 1), Point(0, 0, -1));
	Plane plane(Point(0, 0, 0), Point(0, 1, 1));
	Point p;
//	int k=Sur_elemIntersectedByLine(ray_line, elem_vec, p);


//	std::cout << k << std::endl;

}

/**
 * Figure out whether a Elem is intersected by a line or not.
 *
 * @return true if the Elem intersected by the line.  Will return -1 if it doesn't
 */
bool MonteCarloUserObject::sideIntersectedByLine(const RayLine& ray, const Elem* side, Point& point)
{
	bool intersect(false);
	unsigned int dim = side->dim();
	if (dim == 3)
	{
		Plane plane(*side->get_node(0), *side->get_node(1), *side->get_node(2));
		intersect = ray.intersect(plane, point);
	}
	else if (dim == 2)
	{
		RayLine side_ray(*side->get_node(0), *side->get_node(1));
		intersect = ray.intersect(side_ray, point);
	}

	else // 1D
	{
		intersect = ray.contains_point(*side->get_node(0));
		if (intersect)
		point = *side->get_node(0);
	}

	return intersect;

}

/**
 * Figure out which (if any) Elem is intersected by a line.
 *
 * @return The Elem that is intersected by the line.  Will return -1 if it doesn't intersect any elem
 */
//int MonteCarloUserObject::Sur_elemIntersectedByLine(const RayLine & line_segment, const std::vector<Elem*> elem_vec, Point &intersection_point)
//{
//	unsigned int n_elem = elem_vec.size();
//	int i_in=0;
//
//	for (unsigned int i=0; i<n_elem; i++)
//	{
////		if (elem_vec[i]->contains_point(line_segment.start())) // Don't search the "not_side"
//			continue;
//
//		if(sideIntersectedByLine(line_segment, elem_vec[i], intersection_point))
//		{
//
//		}
//		return i;
//	}
//
//	return -1;   // Didn't find one
//}
