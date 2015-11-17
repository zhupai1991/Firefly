
#pragma once

#include "libmesh/mesh.h"
#include "libmesh/elem.h"
#include "UserDefinedElem.h"
#include <vector>

using std::vector;
using libMesh::Mesh;
class MooseMesh;

class UserDefinedMesh
{
public:
//	UserDefinedElem * Pointlocator( Point & point);
	UserDefinedMesh(const MooseMesh &mesh);
	vector<UserDefinedElem * > _userDefinedElem;

	const MooseMesh &_mesh;

protected:

};
