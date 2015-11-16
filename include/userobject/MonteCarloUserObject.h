
#pragma once

#include "SideUserObject.h"
#include "RandomInterface.h"
#include "SideElement.h"
#include <vector>
using std::vector;

namespace Firefly
{
int findFinalSideId(RayLine & ray_line, Point & point, SideElement * sideelement);
}

class RayLine;

class MonteCarloUserObject :
public SideUserObject,
public RandomInterface
{

public:
	MonteCarloUserObject(const InputParameters & parameters);
	int Which_SideelementIntersectedByLine(RayLine& ray, SideElement * sideelement_i, vector<SideElement*> sideelement_vec, Point & point);
	int Find_j_of_RDij(SideElement * sideelement_i, vector<SideElement*> sideelement_vec);

protected :
	virtual void initialSetup();
	virtual void initialize();
	virtual void finalize(){};
	virtual void execute();
	virtual void threadJoin(const UserObject & uo){};

	vector<SideElement*> _all_element;
	int _max_reflect_count;
	int _particle_count;
	Real _absorptivity;
	Real _diffuse_reflectivity;
	Real _mirrors_reflectivity;

};

template<>
InputParameters validParams<MonteCarloUserObject>();
