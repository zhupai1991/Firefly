
#include "SideElement.h"
#include "RayLine.h"
#include "LineSegment.h"
#include "MooseRandom.h"
//#include "RandomInterface.h"
using namespace std;

SideElement::SideElement(const Elem *elem, const Point normal) :
 Absorptivity(0.5),
 Diffuse_Reflectivity(0.5),
 Mirrors_Reflectivity(0.5),
 MaxReflectCount(10),
 _elem(elem),
 _normal(normal)
{
}

RayLine SideElement::SendRay()
{
//	MooseRandom::seed(0);
	Real theita = 2*pi*MooseRandom::rand();

	unsigned int dim = _elem->dim();
	if (dim == 3)
	{
		Point p = _elem->centroid();
		Point O1 = p+_normal;
		Real phi = acos(1 - 2*MooseRandom::rand());

		Point M(cos(theita)*sin(phi),sin(theita)*sin(phi),cos(phi));
		M+=O1;

		if((M-p).size()<TOLERANCE)
		{
			return RayLine(p,_normal);
		}

		else
		{
			return RayLine(p,(M-p).unit());
		}
	}

	else if (dim == 2)
	{
		Point p = _elem->centroid();
		Point O1 = p+_normal;

		Point M(cos(theita),sin(theita));
		M+=O1;

		if((M-p).size()<TOLERANCE)
		{
			return RayLine(p,_normal);
		}

		else
		{
			std::cout << (M-p).unit() << std::endl;
			cout << "p:" <<p <<endl;
//			std::cout << RayLine(p,_normal).normal << std::endl;
			return RayLine(p,(M-p).unit());
		}
	}

	else // 1D
	{
		Point p = _elem->centroid();
		return RayLine(p,p+100000*_normal);
	}

}

RayLine SideElement::DiffuseReflectRay(RayLine* rayline, Point point)
{
//	MooseRandom::seed(0);
	Real theita = 2*pi*MooseRandom::rand();

	unsigned int dim = _elem->dim();

	if (dim == 3)
	{
		Point O1 = point+_normal;
		Real phi = acos(1 - 2*MooseRandom::rand());

		Point M(cos(theita)*sin(phi),sin(theita)*sin(phi),cos(phi));
		M+=O1;

		if((M-point).size()<TOLERANCE)
		{
			return RayLine(point,_normal);
		}

		else
		{
			return RayLine(point,(M-point).unit());
		}
	}

	else if (dim == 2)
	{
		Point O1 = point+_normal;

		Point M(cos(theita),sin(theita));
		M+=O1;

		if((M-point).size()<TOLERANCE)
		{
			return RayLine(point,_normal);
		}

		else
		{
			return RayLine(point,(M-point).unit());
		}
	}

	else // 1D
	{
		return RayLine(point,_normal);
	}

}

RayLine SideElement::MirrorsReflectRay(RayLine* rayline, Point point)
{
	Point normal_in=rayline->_normal;
	Point normal_out=normal_in+_normal*(2*cos((_normal*normal_in)/(normal_in.size()*_normal.size())));

	return RayLine(point,normal_out);
}
