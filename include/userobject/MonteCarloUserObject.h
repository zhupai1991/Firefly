
#pragma once

#include "SideUserObject.h"
#include "RandomInterface.h"
#include "SideElement.h"
#include <vector>
using std::vector;

class RayLine;

class MonteCarloUserObject :
public SideUserObject,
public RandomInterface
{

public:
	MonteCarloUserObject(const InputParameters & parameters);
	int Which_SideelementIntersectedByLine(RayLine& ray, SideElement * sideelement_i, vector<SideElement*> sideelement_vec, Point point);
	int Find_j_of_RDij(SideElement * sideelement_i, vector<SideElement*> sideelement_vec);

protected :
	virtual void initialize();
	virtual void finalize(){};
	virtual void execute();
	virtual void threadJoin(const UserObject & uo){};

	vector<SideElement*> _all_element;

//	bool sideIntersectedByLine(const RayLine &ray, const Elem *side, Point &point);


};

template<>
InputParameters validParams<MonteCarloUserObject>();
