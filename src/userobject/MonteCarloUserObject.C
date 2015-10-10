
#include "MonteCarloUserObject.h"
#include "FEProblem.h"
#include "libmesh/elem.h"
#include "RayLine.h"
#include "MooseRandom.h"
#include "libmesh/plane.h"

using namespace std;

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

void MonteCarloUserObject::initialize()
{
    vector<BoundaryName> boundary = getParam<std::vector<BoundaryName> >("boundary");
    for(vector<BoundaryName>::iterator it = boundary.begin(); it != boundary.end(); ++it)
    	std::cout << *it <<endl;

    MeshBase & mesh = _mesh.getMesh();
    const BoundaryInfo &bnd_info = mesh.get_boundary_info();
    MeshBase::const_element_iterator   el  = mesh.active_elements_begin();
    const MeshBase::const_element_iterator end_el = mesh.active_elements_end();
    for ( ; el != end_el ; ++el)
     {
       const Elem *elem = *el;

       for (unsigned int side=0; side < elem->n_sides(); ++side)
       {
         if (elem->neighbor(side))
           continue;

         Elem *elem_side = elem->build_side(side).release();
         int bnd_id = bnd_info.boundary_id(elem, side);
         if(bnd_id == 0)
        	 _all_element.push_back(new SideElement(elem_side, Point(0, 1,0)));

         else if(bnd_id == 1)
        	 _all_element.push_back(new SideElement(elem_side, Point(-1,0,0)));

         else if(bnd_id == 2)
        	 _all_element.push_back(new SideElement(elem_side, Point(0,-1,0)));

         else if(bnd_id == 3)
        	 _all_element.push_back(new SideElement(elem_side, Point(1, 0, 0)));

         else
        	 mooseError("boundary id error.");

       }
     }

}

void MonteCarloUserObject::execute()
{

	Real RD[_all_element.size()]={0};
	int p_count=10000000;

	SideElement current_side_element(_current_side_elem,  -_normals[0]);
	SideElement * cse = &current_side_element;

//	cout << "current line:" << endl;
//	RayLine ray_line = current_side_element.SendRay();
//	cout << ray_line;
//
//	Point p;
//
//	int jjj=Which_SideelementIntersectedByLine(ray_line, cse, _all_element, p);
//	int jjj=Find_j_of_RDij(cse, _all_element);
//	cout << "jjj:" << jjj << endl;
//	cout << "point:" << _all_element[jjj]->_elem->centroid() << endl;

	for (int i=0;i<p_count;i++)
	{
		int j_of_RDij=-1;

		j_of_RDij=Find_j_of_RDij(cse, _all_element);

		if (j_of_RDij == -1)
			continue;

		else
			RD[j_of_RDij]=RD[j_of_RDij]+1.0;
	}

	cout << "qwertyu" << endl;
	for (int i=0;i<_all_element.size();i++)
	{
		RD[i]=RD[i]/p_count;
		cout << "_all_element:" << _all_element[i]->_elem->centroid() << "        RD:" << RD[i] << endl;
	}

//	for (int i=0 ;i<1 ; i++)
//	{
//		int jjjjjjj=Find_j_of_RDij(cse, _all_element);
//		cout << "jjjjjjj:" << jjjjjjj << endl;
//		cout << "point:" << _all_element[jjjjjjj]->_elem->centroid() << endl;
//	}
//
//	for(int i  = 0; i < _all_element.size(); ++i)
//	{
//		if(ray_line.sideIntersectedByLine(_all_element[i]->_elem, p))
//		{
//			cout << ray_line ;
//			cout << p << endl;
//		}
//	}

}


int MonteCarloUserObject::Which_SideelementIntersectedByLine(RayLine& ray, SideElement * sideelement_i, vector<SideElement*> sideelement_vec, Point & point)
{
	int j_max=sideelement_vec.size();
	int j_wanted=-1;
	Point pp=ray._p0;
	point=ray._p1;

	for(int j=0; j<j_max; j++)
	{
//		if(sideelement_vec[j] == sideelement_i)
		if( (sideelement_vec[j]->_elem->centroid()-sideelement_i->_elem->centroid()).size()<TOLERANCE )
			continue;

		else if(!(ray.sideIntersectedByLine(sideelement_vec[j]->_elem,pp)))
			continue;

		else if((pp-ray._p0).size()>(point-ray._p0).size())
			continue;

		else
		{
			j_wanted=j;
			point=pp;
		}
	}
//	cout << "in_Which_SideelementIntersectedByLine:" << point << endl;
	return j_wanted;
}

int MonteCarloUserObject::Find_j_of_RDij(SideElement * sideelement_i, vector<SideElement*> sideelement_vec)
{
	int j=0;
	int j_of_RDij=-1;
	int k=0;
	bool charge=true;
//	vector<SideElement*> sideelement_ve=sideelement_vec;
	SideElement * current_elem= sideelement_i;
	RayLine rayline_in;
	RayLine rayline_out;
	RayLine* ray_in=&rayline_in;
	RayLine* ray_out=&rayline_out;
	Point p(0,0,0);

	rayline_in=(*current_elem).SendRay();

	while (charge && (k < sideelement_i->MaxReflectCount) )
	{


//		cout << "rayline_in:" << rayline_in << endl;

		j=Which_SideelementIntersectedByLine( rayline_in, current_elem, sideelement_vec, p);
//		cout << "j:" << j << endl;
//		cout << "p:" << p << endl;

		if(j==-1)
			return -1;

		else if(MooseRandom::rand()<sideelement_vec[j]->Absorptivity)
		{
//			cout << "Absorptivity" << endl;
			charge=false;
			j_of_RDij=j;
			break;
		}

		else if(MooseRandom::rand()<sideelement_vec[j]->Diffuse_Reflectivity)
		{
//			cout << "Diffuse_Reflectivity" << endl;
			rayline_out=sideelement_vec[j]->DiffuseReflectRay(ray_in,p);
			current_elem=sideelement_vec[j];
			rayline_in=rayline_out;
			j_of_RDij=j;
			k++;
			continue;
		}

		else
		{
//			cout << "Mirrors_ReflectRay" << endl;
			rayline_out=sideelement_vec[j]->MirrorsReflectRay(ray_in,p);
			current_elem=sideelement_vec[j];
			rayline_in=rayline_out;
			j_of_RDij=j;
			k++;
			continue;
		}
	}

//	cout << "k:" << k << endl;

	if(!charge)
		return j_of_RDij;

	else if(k==sideelement_i->MaxReflectCount)
		return j_of_RDij;

	else
		return -1;
}
