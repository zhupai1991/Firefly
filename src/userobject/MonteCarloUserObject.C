
#include "MonteCarloUserObject.h"
#include "FEProblem.h"
#include "libmesh/elem.h"
#include "libmesh/fe.h"
#include "libmesh/string_to_enum.h"
#include "libmesh/quadrature_gauss.h"
#include "libmesh/plane.h"
#include "MooseRandom.h"

#include "RayLine.h"
#include "UserDefinedMesh.h"
#include "UserDefinedElem.h"
#include "UserDefinedSideElem.h"

using namespace std;
using namespace Firefly;

template<>
InputParameters validParams<MonteCarloUserObject>()
{
	InputParameters params = validParams<SideUserObject>();
	params += validParams<RandomInterface>();
	params.addParam<int> ("max_reflect_count", 10, "最大反射次数");
	params.addParam<int> ("particle_count", 1000000, "单元发射粒子数");
	params.addParam<Real> ("absorptivity", 0.5, "吸收率");
	params.addParam<Real> ("diffuse_reflectivity", 0.5, "漫反射百分比");
	params.addParam<Real> ("mirrors_reflectivity", 0.5, "镜反射百分比");
	return params;
}

MonteCarloUserObject::MonteCarloUserObject(const InputParameters & parameters) :
	SideUserObject(parameters),
	RandomInterface(parameters, *parameters.get<FEProblem *>("_fe_problem"), parameters.get<THREAD_ID>("_tid"), false),
	_max_reflect_count(getParam<int> ("max_reflect_count")),
	_particle_count(getParam<int> ("particle_count")),
	_absorptivity(getParam<Real> ("absorptivity")),
	_diffuse_reflectivity(getParam<Real> ("diffuse_reflectivity")),
	_mirrors_reflectivity(getParam<Real> ("mirrors_reflectivity"))
{
}

//void MonteCarloUserObject::initialSetup()
//{
//	vector<BoundaryName> boundary = getParam<std::vector<BoundaryName> >("boundary");
//	std::set<BoundaryID> boundary_ids;
//	for(vector<BoundaryName>::iterator it = boundary.begin(); it != boundary.end(); ++it)
//	{
//		BoundaryID id = _mesh.getBoundaryID(*it);
//		boundary_ids.insert(id);
//		//    	std::cout << id <<endl;
//	}
//
//	MeshBase & mesh = _mesh.getMesh();
//	const BoundaryInfo &bnd_info = mesh.get_boundary_info();
//	MeshBase::const_element_iterator   el  = mesh.active_elements_begin();
//	const MeshBase::const_element_iterator end_el = mesh.active_elements_end();
//	for ( ; el != end_el ; ++el)
//	{
//		const Elem *elem = *el;
//
//		for (unsigned int side=0; side < elem->n_sides(); ++side)
//		{
//			if (elem->neighbor(side))
//				continue;
//
//			Elem *elem_side = elem->build_side(side).release();
//			int bnd_id = bnd_info.boundary_id(elem, side);
//			if(find(boundary_ids.begin(), boundary_ids.end(), bnd_id) == boundary_ids.end())
//				continue;
//
//			unsigned int dim = _mesh.dimension();
//			FEType fe_type(Utility::string_to_enum<Order>("CONSTANT"), Utility::string_to_enum<FEFamily>("MONOMIAL"));
//			FEBase * _fe_face = (FEBase::build(dim, fe_type)).release();
//			QGauss * _qface = new QGauss(dim-1, FIRST);
//			_fe_face->attach_quadrature_rule(_qface);
//			_fe_face->reinit(elem, side);
//			const std::vector<Point> normals = _fe_face->get_normals();
//
//			_all_element.push_back(new SideElement(elem_side, -normals[0], _absorptivity, _diffuse_reflectivity, _mirrors_reflectivity));
//
//		}
//	}
//	//		cout << this << endl;
//}

void MonteCarloUserObject::initialSetup()
{
	vector<BoundaryName> boundary = getParam<std::vector<BoundaryName> >("boundary");
	std::set<BoundaryID> boundary_ids;
	for(vector<BoundaryName>::iterator it = boundary.begin(); it != boundary.end(); ++it)
	{
		BoundaryID id = _mesh.getBoundaryID(*it);
		boundary_ids.insert(id);
	}

	int count_sideelement = 0;

	MeshBase & mesh = _mesh.getMesh();
	UserDefinedMesh * mymesh = new UserDefinedMesh(_mesh);  //**************<--这里有new***************//

	const BoundaryInfo &bnd_info = mesh.get_boundary_info();
//	MeshBase::const_element_iterator el = mesh.active_elements_begin();
//	const MeshBase::const_element_iterator end_el = mesh.active_elements_end();
	int nelems = mesh.n_elem();

	mymesh->_userDefinedElem.resize(nelems);
	for (int nelem =0; nelem<nelems; nelem++)
	{
		UserDefinedElem * newelem = new UserDefinedElem;    //**************<--这里有new***************//
		mymesh->_userDefinedElem[nelem] = newelem;
	}

	for (int nelem =0; nelem<nelems; nelem++)
	{
		const Elem * elem = mesh.elem(nelem);
		UserDefinedElem * myelem = mymesh->_userDefinedElem[nelem];
//		mymesh->_userDefinedElem[nelem]->_elem = elem;
		myelem->_elem = elem;
		int nsides = mesh.elem(nelem)->n_sides();
//		mymesh->_userDefinedElem[nelem]->_userDefinedSideElem.resize(nsides);
		myelem->_userDefinedSideElem.resize(nsides);
		for (int nside=0; nside < nsides; nside++)
		{
			UserDefinedSideElem * newsideelem = new UserDefinedSideElem;    //**************<--这里有new***************//
//			mymesh->_userDefinedElem[nelem]->_userDefinedSideElem[nside] = newsideelem;
			myelem->_userDefinedSideElem[nside] = newsideelem;
		}
//		mymesh->_userDefinedElem[nelem]->_haveWhichSideElement.resize(nsides, -1);
		myelem->_haveWhichSideElement.resize(nsides, -1);

		for (int nside=0; nside < nsides; nside++)
		{
			UserDefinedSideElem * mysideelem = mymesh->_userDefinedElem[nelem]->_userDefinedSideElem[nside];
//			mymesh->_userDefinedElem[nelem]->_userDefinedSideElem[nside]._elem = elem->side(nside);
//			mymesh->_userDefinedElem[nelem]->_userDefinedSideElem[nside]._left_element = mymesh->_userDefinedElem[nelem];
			mysideelem->_elem = elem->side(nside).release();
			mysideelem->_left_element = mymesh->_userDefinedElem[nelem];

			if (elem->neighbor(nside))
			{
//				mymesh->_userDefinedElem[nelem]->_userDefinedSideElem[nside]._right_element = mymesh->_userDefinedElem[elem->neighbor(nside)->_id];
				mysideelem->_right_element = mymesh->_userDefinedElem[elem->neighbor(nside)->id()];
			}

			else
			{
//				mymesh->_userDefinedElem[nelem]->_userDefinedSideElem[nside]._right_element = NULL;
				mysideelem->_right_element = NULL;

				Elem *elem_side = elem->build_side(nside).release();
				int bnd_id = bnd_info.boundary_id(elem, nside);
				if(find(boundary_ids.begin(), boundary_ids.end(), bnd_id) == boundary_ids.end())
					continue;

				unsigned int dim = _mesh.dimension();
				FEType fe_type(Utility::string_to_enum<Order>("CONSTANT"), Utility::string_to_enum<FEFamily>("MONOMIAL"));
				FEBase * _fe_face = (FEBase::build(dim, fe_type)).release();
				QGauss * _qface = new QGauss(dim-1, FIRST);
				_fe_face->attach_quadrature_rule(_qface);
				_fe_face->reinit(elem, nside);
				const std::vector<Point> normals = _fe_face->get_normals();
				SideElement * newsideelement = new SideElement(elem_side, -normals[0], _absorptivity, _diffuse_reflectivity, _mirrors_reflectivity);    //**************<--这里有new***************//
				_all_element.push_back(newsideelement);
				_all_element[count_sideelement]->_belong_to_which_elem = myelem;
				_all_element[count_sideelement]->_is_which_sideelem = nside;
				myelem->_haveWhichSideElement[nside] = count_sideelement;
//				cout << count_sideelement << endl;
				count_sideelement++;

				delete _fe_face;
				delete _qface;
			}
		}
	}

	_n=0;
//	MooseRandom::seed(0.7);

}

void MonteCarloUserObject::initialize()
{
//    vector<BoundaryName> boundary = getParam<std::vector<BoundaryName> >("boundary");
//    std::set<BoundaryID> boundary_ids;
//    for(vector<BoundaryName>::iterator it = boundary.begin(); it != boundary.end(); ++it)
//    {
//    	BoundaryID id = _mesh.getBoundaryID(*it);
//    	boundary_ids.insert(id);
////    	std::cout << id <<endl;
//    }
//
//    MeshBase & mesh = _mesh.getMesh();
//    const BoundaryInfo &bnd_info = mesh.get_boundary_info();
//    MeshBase::const_element_iterator   el  = mesh.active_elements_begin();
//    const MeshBase::const_element_iterator end_el = mesh.active_elements_end();
//    for ( ; el != end_el ; ++el)
//     {
//       const Elem *elem = *el;
//
//       for (unsigned int side=0; side < elem->n_sides(); ++side)
//       {
//         if (elem->neighbor(side))
//           continue;
//
//         Elem *elem_side = elem->build_side(side).release();
//         int bnd_id = bnd_info.boundary_id(elem, side);
//         if(find(boundary_ids.begin(), boundary_ids.end(), bnd_id) == boundary_ids.end())
//        	 continue;
//
//         unsigned int dim = _mesh.dimension();
//         FEType fe_type(Utility::string_to_enum<Order>("CONSTANT"), Utility::string_to_enum<FEFamily>("MONOMIAL"));
//         FEBase * _fe_face = (FEBase::build(dim, fe_type)).release();
//         QGauss * _qface = new QGauss(dim-1, FIRST);
//         _fe_face->attach_quadrature_rule(_qface);
//         _fe_face->reinit(elem, side);
//         const std::vector<Point> normals = _fe_face->get_normals();
//
//         _all_element.push_back(new SideElement(elem_side, -normals[0], _absorptivity, _diffuse_reflectivity, _mirrors_reflectivity));
//
//       }
//     }
//	cout << this << endl;
//	mooseError("d");
}

void MonteCarloUserObject::execute()
{
	Real RD[_all_element.size()]={0};

//	SideElement current_side_element(_current_side_elem,  -_normals[0], _absorptivity, _diffuse_reflectivity, _mirrors_reflectivity);
//	SideElement * cse = &current_side_element;

	SideElement * cse = _all_element[_n];

	for (int i=0;i<_particle_count;i++)
	{
		int j_of_RDij=-1;

		j_of_RDij=Find_j_of_RDij(cse, _all_element);

		if (j_of_RDij == -1)
			continue;

		else
			RD[j_of_RDij]=RD[j_of_RDij]+1.0;
	}

	cout << endl << "单元计算结果：" << endl;
//	cout << "当前单元中心点：" << _current_side_elem->centroid() <<endl;
	cout << "当前单元中心点：" << _all_element[_n]->_elem->centroid() <<endl;
	for (int i=0;i<_all_element.size();i++)
	{
		RD[i]=RD[i]/_particle_count;
//		cout << "side_element_centre:" << _all_element[i]->_elem->centroid() << "        RD:" << RD[i] << endl;
		cout << RD[i] << endl;
	}
//	mooseError("产生随机位置时不支持的网格形状：");

	_n++;

//	RayLine rayline=RayLine(_current_side_elem->centroid()-(1e-4)*_normals[0], -_normals[0]);
//	cout << "_current_side_elem->centroid():" << _current_side_elem->centroid() << endl;
//	cout << "rayline_normal:" << rayline._normal << endl;
//	cout << "P1:" << rayline._p1 << endl;
////	RayLine ray1( Point(0.5,0,1),Point(0,0.5,1), 1);
//	Point point;
////	bool i= rayline.intersect(ray1,point);
////	cout << "i:" << i << endl;
//	int i= findFinalSideId(rayline, _mesh.getMesh(), point, _all_element);
//	cout << "i:" << i << endl;
//	cout << "Point:" << point << endl;
}


//int MonteCarloUserObject::Which_SideelementIntersectedByLine(RayLine& ray, SideElement * sideelement_i, vector<SideElement*> sideelement_vec, Point & point)
//{
//	int j_max=sideelement_vec.size();
//	int j_wanted=-1;
//	Point pp=ray._p0;
//	point=ray._p1;
//
//	for(int j=0; j<j_max; j++)
//	{
////		if(sideelement_vec[j] == sideelement_i)
//		if( (sideelement_vec[j]->_elem->centroid()-sideelement_i->_elem->centroid()).size()<TOLERANCE )
////		if( ((sideelement_vec[j]->_elem->centroid()-sideelement_i->_elem->centroid()).size() < TOLERANCE) || ((sideelement_vec[j]->_normal)*(sideelement_i->_normal) > TOLERANCE)  )
//			continue;
//
//		else if(!(ray.sideIntersectedByLine(sideelement_vec[j]->_elem,pp)))
//			continue;
//
//		else if((pp-ray._p0).size()>(point-ray._p0).size())
//			continue;
//
//		else
//		{
//			j_wanted=j;
//			point=pp;
//		}
//	}
////	cout << "in_Which_SideelementIntersectedByLine:" << point << endl;
//	return j_wanted;
//}
//
//int MonteCarloUserObject::Find_j_of_RDij(SideElement * sideelement_i, vector<SideElement*> sideelement_vec)
//{
//	int j=0;
//	int j_of_RDij=-1;
//	int k=0;
//	bool charge=true;
////	vector<SideElement*> sideelement_ve=sideelement_vec;
//	SideElement * current_elem= sideelement_i;
//	RayLine rayline_in;
//	RayLine rayline_out;
//	RayLine* ray_in=&rayline_in;
//	RayLine* ray_out=&rayline_out;
//	Point p(0,0,0);
//
//	rayline_in=(*current_elem).sendRay();
////	cout << rayline_in._normal << endl;
//	while (charge && (k < _max_reflect_count) )
//	{
////		cout << "rayline_in:" << rayline_in << endl;
//
//		j=Which_SideelementIntersectedByLine( rayline_in, current_elem, sideelement_vec, p);
////		cout << "j:" << j << endl;
////		cout << "p:" << p << endl;
//
//		if(j==-1)
//			return -1;
//
//		else if(MooseRandom::rand()<sideelement_vec[j]->_absorptivity)
//		{
////			cout << "Absorptivity" << endl;
//			charge=false;
//			j_of_RDij=j;
//			break;
//		}
//
//		else if(MooseRandom::rand()<sideelement_vec[j]->_diffuse_reflectivity)
//		{
////			cout << "Diffuse_Reflectivity" << endl;
//			rayline_out=sideelement_vec[j]->diffuseReflectRay(ray_in,p);
//			current_elem=sideelement_vec[j];
//			rayline_in=rayline_out;
//			j_of_RDij=j;
//			k++;
//			continue;
//		}
//
//		else
//		{
////			cout << "Mirrors_ReflectRay" << endl;
//			rayline_out=sideelement_vec[j]->mirrorsReflectRay(ray_in,p);
//			current_elem=sideelement_vec[j];
//			rayline_in=rayline_out;
//			j_of_RDij=j;
//			k++;
//			continue;
//		}
//	}
//
////	cout << "k:" << k << endl;
//
//	if(!charge)
//		return j_of_RDij;
//
//	else if(k == _max_reflect_count)
//		return j_of_RDij;
//
//	else
//		return -1;
//}

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

	rayline_in=(*current_elem).sendRay();
//	rayline_in=RayLine(rayline_in._p0+2*TOLERANCE*rayline_in._normal, rayline_in._normal);

//	cout << rayline_in._normal << endl;
	while (charge && (k < _max_reflect_count) )
	{
//		cout << "rayline_in:" << rayline_in << endl;

		j=findFinalSideId(rayline_in, p, current_elem);

//		cout << "j:" << j << endl;
//		cout << "p:" << p << endl;

		if(j==-1)
			return -1;

		else if(MooseRandom::rand()<=sideelement_vec[j]->_absorptivity)
		{
//			cout << "Absorptivity" << endl;
			charge=false;
			j_of_RDij=j;
			break;
		}

		else if(MooseRandom::rand()<=sideelement_vec[j]->_diffuse_reflectivity)
		{
//			cout << "Diffuse_Reflectivity" << endl;
			rayline_out=sideelement_vec[j]->diffuseReflectRay(ray_in,p);
			current_elem=sideelement_vec[j];
			rayline_in=rayline_out;
			j_of_RDij=j;
			k++;
			continue;
		}

		else
		{
//			cout << "Mirrors_ReflectRay" << endl;
			rayline_out=sideelement_vec[j]->mirrorsReflectRay(ray_in,p);
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

	else if(k == _max_reflect_count)
		return j_of_RDij;

	else
		return -1;
}

namespace Firefly
{

int sideIntersectedByLine(const Elem * elem, int not_side, RayLine & ray_line, Point & intersection_point)
{
	unsigned int n_sides = elem->n_sides();

	// A Point to pass to the intersection method
	//  Point intersection_point;

	// Whether or not they intersect
	bool intersect = false;

	unsigned int dim = elem->dim();

	for (unsigned int i=0; i<n_sides; i++)
	{
		if (static_cast<int>(i) == not_side) // Don't search the "not_side"
			continue;

		// Get a simplified side element
		UniquePtr<Elem> side_elem = elem->side(i);

		if (dim == 3)
		{
			// Make a plane out of the first three nodes on the side
			Plane plane(side_elem->point(0), side_elem->point(1), side_elem->point(2));

			// See if they intersect
			intersect = ray_line.intersect(plane, intersection_point);
		}
		else if (dim == 2)
		{
			// Make a Line Segment out of the first two nodes on the side
			RayLine ray_line_side(side_elem->point(0), side_elem->point(1), 1);

			// See if they intersect
			intersect = ray_line.intersect(ray_line_side, intersection_point);
		}
		else // 1D
		{
			// See if the line segment contains the point
			intersect = ray_line.contains_point(side_elem->point(0));

			// If it does then save off that one point as the intersection point
			if (intersect)
				intersection_point = side_elem->point(0);
		}

		if (intersect)
			if (side_elem->contains_point(intersection_point, 1e-12))
				return i;
	}

	// Didn't find one
	return -1;
}

/**
 * Returns the side number for elem that neighbor is on
 *
 * Returns -1 if the neighbor can't be found to be a neighbor
 */
int sideNeighborIsOn(const UserDefinedElem * elem, UserDefinedElem * neighbor)
{
  unsigned int n_sides = neighbor->_elem->n_sides();

  for (unsigned int i=0; i<n_sides; i++)
  {
	  if (neighbor->_userDefinedSideElem[i]->_right_element == elem)
		  return i;
  }

  return -1;
}

int pointInWhichSide(const Elem * elem, Point & point)
{
	unsigned int n_sides = elem->n_sides();

	for (unsigned int i=0; i<n_sides; i++)
	{
		if( (*(elem->side(i))).contains_point(point) )
			return i;
	}
	return -1;
}

int findFinalSideId(RayLine & ray_line, Point & point, SideElement * sideelement)
//Point findFinalSideId(RayLine & ray_line, const MeshBase & mesh, Point & point)
{
	int incoming_side = -1;
	UserDefinedElem * neighbor;

	const UserDefinedElem * first_elem = sideelement->_belong_to_which_elem;

	if (!first_elem)
	{
		cout << "first_elem" << endl;
		return -1;
	}

	else
	{
		const UserDefinedElem * current_elem = first_elem;
		incoming_side = sideelement->_is_which_sideelem;
//		cout << incoming_side << endl;
		bool charge = true;
		while (charge)
		{
			int intersected_side = sideIntersectedByLine(current_elem->_elem, incoming_side, ray_line, point);

			if (intersected_side != -1) // -1 means that we didn't find any side
			{
				neighbor = current_elem->_userDefinedSideElem[intersected_side]->_right_element;

//				cout << "neighbor" << endl;
				if (neighbor)
				{
//					cout << "intersecedside:" << intersected_side << endl;
					incoming_side = sideNeighborIsOn(current_elem, neighbor);
					current_elem = neighbor;
				}

				else
				{
					return current_elem->_haveWhichSideElement[intersected_side];
				}
			}
			else
			{
//				cout << "else" << endl;
				return -1;
			}
		}
	}
	return -1;
}
}

//namespace Firefly
//{
//
//int sideIntersectedByLine(const Elem * elem, int not_side, RayLine & ray_line, Point & intersection_point)
//{
//	unsigned int n_sides = elem->n_sides();
//
//	// A Point to pass to the intersection method
//	//  Point intersection_point;
//
//	// Whether or not they intersect
//	bool intersect = false;
//
//	unsigned int dim = elem->dim();
//
//	for (unsigned int i=0; i<n_sides; i++)
//	{
//		if (static_cast<int>(i) == not_side) // Don't search the "not_side"
//			continue;
//
//		// Get a simplified side element
//		UniquePtr<Elem> side_elem = elem->side(i);
//
//		if (dim == 3)
//		{
//			// Make a plane out of the first three nodes on the side
//			Plane plane(side_elem->point(0), side_elem->point(1), side_elem->point(2));
//
//			// See if they intersect
//			intersect = ray_line.intersect(plane, intersection_point);
//		}
//		else if (dim == 2)
//		{
//			// Make a Line Segment out of the first two nodes on the side
//			RayLine ray_line_side(side_elem->point(0), side_elem->point(1), 1);
//
//			// See if they intersect
//			intersect = ray_line.intersect(ray_line_side, intersection_point);
//		}
//		else // 1D
//		{
//			// See if the line segment contains the point
//			intersect = ray_line.contains_point(side_elem->point(0));
//
//			// If it does then save off that one point as the intersection point
//			if (intersect)
//				intersection_point = side_elem->point(0);
//		}
//
//		if (intersect)
//			if (side_elem->contains_point(intersection_point))
//				return i;
//	}
//
//	// Didn't find one
//	return -1;
//}
//
///**
// * Returns the side number for elem that neighbor is on
// *
// * Returns -1 if the neighbor can't be found to be a neighbor
// */
//int sideNeighborIsOn(const Elem * elem, const Elem * neighbor)
//{
//  unsigned int n_sides = elem->n_sides();
//
//  for (unsigned int i=0; i<n_sides; i++)
//  {
//    if (elem->neighbor(i) == neighbor)
//      return i;
//  }
//
//  return -1;
//}
//
//int pointInWhichSide(const Elem * elem, Point & point)
//{
//	unsigned int n_sides = elem->n_sides();
//
//	for (unsigned int i=0; i<n_sides; i++)
//	{
//		if( (*(elem->side(i))).contains_point(point) )
//			return i;
//	}
//	return -1;
//}
//
//int findFinalSideId(RayLine & ray_line, const MeshBase & mesh, Point & point, vector<SideElement*> sideelement_vec)
////Point findFinalSideId(RayLine & ray_line, const MeshBase & mesh, Point & point)
//
//{
//	int incoming_side = -1;
//	Elem * neighbor;
//	const PointLocatorBase & pl = mesh.point_locator();
////	const Elem * first_elem = pl(ray_line._p0+(1e-4)*ray_line._normal);
//	const Elem * first_elem = pl(ray_line._p0);
//
//	if (!first_elem)
//	{
//		cout << "first_elem" << endl;
//		return -1;
//	}
//
//	else
//	{
//		const Elem * current_elem = first_elem;
//		incoming_side = pointInWhichSide(current_elem, ray_line._p0);
//		while (true)
//		{
//			int intersected_side = sideIntersectedByLine(current_elem, incoming_side, ray_line, point);
////			cout << "intersecedside:" << intersected_side << endl;
//
//			if (intersected_side != -1) // -1 means that we didn't find any side
//			{
//				neighbor = current_elem->neighbor(intersected_side);
//
////				cout << "neighbor" << endl;
//				if (neighbor)
//				{
//					incoming_side = sideNeighborIsOn(neighbor, current_elem);
//					current_elem = neighbor;
//				}
//
//				else
//				{
////					UniquePtr<Elem> side_elem = current_elem->side(intersected_side);
////					return side_elem->centroid();
//					for (int j=0;j<sideelement_vec.size();j++)
//					{
//						if(sideelement_vec[j]->_elem->contains_point(point,TOLERANCE))
//							return j;
//					}
//
////					cout << current_elem->side(intersected_side)->id() << endl;
////					return current_elem->side(intersected_side)->id();
//				}
//			}
//			else
//			{
////				cout << "else" << endl;
//				return -1;
//			}
//		}
//	}
//}
//}
