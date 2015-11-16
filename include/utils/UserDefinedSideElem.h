
#pragma once

#include "libmesh/mesh.h"
#include "libmesh/elem.h"
#include <vector>

using std::vector;
using libMesh::Elem;

class UserDefinedElem;

class UserDefinedSideElem
{
public:
	UserDefinedElem* _left_element;
	UserDefinedElem* _right_element;
	const Elem *_elem;

protected:

};
