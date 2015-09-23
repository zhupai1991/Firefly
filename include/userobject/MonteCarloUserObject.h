
#pragma once

#include "SideUserObject.h"
#include "RandomInterface.h"
#include <vector>
using std::vector;

class RayLine;

class MonteCarloUserObject :
public SideUserObject,
public RandomInterface
{

public:
	MonteCarloUserObject(const InputParameters & parameters);

protected :
	virtual void initialize(){};
	virtual void finalize(){};
	virtual void execute();
	virtual void threadJoin(const UserObject & uo){};

	bool sideIntersectedByLine(const RayLine &ray, const Elem *side, Point &point);


//	int Sur_elemIntersectedByLine(const RayLine & line_segment, const std::vector<Elem*> elem_vec, Point &intersection_point);


};

template<>
InputParameters validParams<MonteCarloUserObject>();
