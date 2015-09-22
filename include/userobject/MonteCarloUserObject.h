
#pragma once

#include "SideUserObject.h"

class MonteCarloUserObject : public SideUserObject
{

public:
	MonteCarloUserObject(const InputParameters & parameters);

protected :
	virtual void initialize(){};
	virtual void finalize(){};
	virtual void execute();
	virtual void threadJoin(const UserObject & uo){};

};

template<>
InputParameters validParams<MonteCarloUserObject>();
