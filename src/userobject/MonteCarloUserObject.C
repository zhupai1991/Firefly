
#include "MonteCarloUserObject.h"
#include "FEProblem.h"
#include "libmesh/elem.h"
#include "RayLine.h"
#include "MooseRandom.h"
#include "libmesh/plane.h"



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

	std::cout << side_element.SendRay()._normal << std::endl;
//
//	int particle_count=1000000;
//	vector<const Elem*> elem_vec;
//	int n_elem=elem_vec.size();
//	float RD[n_elem][n_elem];
//
//	for(int i=0;i<particle_count;i++)
//
//	vector<const Elem*> elem_vec;
//	elem_vec.push_back(_current_elem);
//
//	Point p;
//	RayLine ray_line(Point(1, 0, 1), Point(0, 0, -1),10000);
//	std::cout << ray_line.normal << std::endl;
//	bool a=ray_line.sideIntersectedByLine(ray_line,elem_vec[0],p);
//	std::cout << p << std::endl;

}







//int MonteCarloUserObject::Which_SideelementIntersectedByLine(const RayLine& ray, SideElement * sideelement_i, vector<const SideElement*> sideelement_vec, Point point)
//{
//	int j_max=sideelement_vec.size();
//	int j_wanted=-1;
//	SideElement * elem= sideelement_i;
//	Point p=ray._p0;
//	point=ray._p1;
//
//	for(int j=0; j<j_max; j++)
//	{
//		if(sideelement_vec[j] == elem)
//			continue;
//
//		if(!(ray.sideIntersectedByLine(ray,sideelement_vec[j],p)))
//			continue;
//
//		else if((p-ray.p0).size()>(point-ray.p0).size())
//			continue;
//
//		else
//		{
//			j_wanted=j;
//			point=p;
//		}
//	}
//
//	return j_wanted;
//}


//int MonteCarloUserObject::Find_j_of_RDij(SideElement * sideelement_i, vector<const SideElement*> sideelement_vec)
//{
//	int j=-1;
//	int j_of_RDij;
//	int k=0;
//	bool charge=true;
//	SideElement * elem= sideelement_i;
//	const RayLine rayline_in;
//	const RayLine rayline_out;
//	Point p;
//
//	while (charge || (k < sideelement_i->MaxReflectCount) )
//	{
//		rayline_in=elem->SendRay();
//
//		j=Which_SideelementIntersectedByLine(rayline_in, sideelement_i, sideelement_vec, p);
//
//		if(j==-1)
//			return -1;
//
//		else if(MooseRandom::rand()<sideelement_vec[j]->Absorptivity)
//		{
//			charge=false;
//			j_of_RDij=j;
//			break;
//		}
//
//		else if(MooseRandom::rand()<sideelement_vec[j]->Diffuse_Reflectivity)
//		{
//			rayline_out=sideelement_vec[j]->DiffuseReflectRay(rayline_in,p);
//			rayline_in=rayline_out;
//			j_of_RDij=j;
//			k++;
//			continue;
//		}
//
//		else
//		{
//			rayline_out=sideelement_vec[j]->MirrorsReflectRay(rayline_in,p);
//			rayline_in=rayline_out;
//			j_of_RDij=j;
//			k++;
//			break;
//		}
//	}
//
//	if(!charge)
//		return j_of_RDij;
//
//	else if(k==sideelement_i->MaxReflectCount)
//		return j_of_RDij;
//
//}
