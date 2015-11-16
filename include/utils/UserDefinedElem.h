
#pragma once

#include "libmesh/mesh.h"
#include "libmesh/elem.h"
#include "UserDefinedSideElem.h"
#include <vector>

using std::vector;
using libMesh::Elem;

class UserDefinedElem
{
public:
	vector<UserDefinedSideElem * > _userDefinedSideElem;
	vector<int> _haveWhichSideElement;
    const Elem * _elem;

protected:

};
