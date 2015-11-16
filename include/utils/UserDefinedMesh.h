
#pragma once

#include "libmesh/mesh.h"
#include "libmesh/elem.h"
#include "UserDefinedElem.h"
#include <vector>

using std::vector;
using libMesh::Mesh;

class UserDefinedMesh
{
public:
//	UserDefinedElem * Pointlocator( Point & point);

	vector<UserDefinedElem * > _userDefinedElem;

	const Mesh * _mesh;

protected:

};
