
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

         Elem *elem_side = elem->side(side).get();

         cout << bnd_info.boundary_id(elem, side) << endl;
       }
     }
//	_all_element.push_back();
}

void MonteCarloUserObject::execute()
{
	std::cout << "start" << std::endl;
	vector<const SideElement*> sideelement_vec;
	sideelement_vec.clear();
	SideElement current_side_element(_current_elem, -_normals[0]);
	sideelement_vec.push_back(&current_side_element);
	int n_elem=sideelement_vec.size();
	int j_of_RDij=-1;
	float RD[n_elem][n_elem]={0};

	int particle_count=5;
	for(int j=0;j<particle_count;j++)
	{
		cout << "11111111111111" << endl;
		j_of_RDij=Find_j_of_RDij( &current_side_element, sideelement_vec);
		cout << "22222222222222" << endl;
		if( j_of_RDij == -1 )
			continue;

		else
		{

			RD[current_side_element._elem->id()][j_of_RDij]++;}
	}

	for(int i=0;i<n_elem;i++)
	{
		cout << "skdjfasfhashfalsjflajf" << endl;
//		RD[current_side_element._elem->id()][i]/=particle_count;
	}

	std::cout << "endmain" << std::endl;
}



int MonteCarloUserObject::Which_SideelementIntersectedByLine(RayLine& ray, SideElement * sideelement_i, vector<const SideElement*> sideelement_vec, Point point)
{
	int j_max=sideelement_vec.size();
	int j_wanted=-1;
	Point pp=ray._p0;
	point=ray._p1;

	for(int j=0; j<j_max; j++)
	{
		if(sideelement_vec[j] == sideelement_i)
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

	return j_wanted;
}


int MonteCarloUserObject::Find_j_of_RDij(SideElement * sideelement_i, vector<const SideElement*> sideelement_vec)
{
	int j=0;
	int j_of_RDij=-1;
	int k=0;
	bool charge=true;
	vector<const SideElement*> sideelement_ve=sideelement_vec;
	SideElement * elem= sideelement_i;
	RayLine* rayline_in;
	RayLine* rayline_out;
	Point p(0);

	while (charge || (k < sideelement_i->MaxReflectCount) )
	{
		(*rayline_in)=elem->SendRay();

		j=Which_SideelementIntersectedByLine(*rayline_in, sideelement_i, sideelement_ve, p);

		if(j==-1)
			return -1;

		else if(MooseRandom::rand()<sideelement_vec[j]->Absorptivity)
		{
			charge=false;
			j_of_RDij=j;
			break;
		}

		else if(MooseRandom::rand()<sideelement_vec[j]->Diffuse_Reflectivity)
		{
			(*rayline_out)=const_cast<SideElement*> (sideelement_vec[j])->DiffuseReflectRay(rayline_in,p);
			rayline_in=rayline_out;
			j_of_RDij=j;
			k++;
			continue;
		}

		else
		{
			(*rayline_out)=const_cast<SideElement*> (sideelement_vec[j])->MirrorsReflectRay(rayline_in,p);
			rayline_in=rayline_out;
			j_of_RDij=j;
			k++;
			continue;
		}
	}

	if(!charge)
		return j_of_RDij;

	else if(k==sideelement_i->MaxReflectCount)
		return j_of_RDij;

	else
		return -1;
}
