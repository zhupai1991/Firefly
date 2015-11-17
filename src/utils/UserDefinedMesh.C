#include "UserDefinedMesh.h"
#include "UserDefinedElem.h"
#include "MooseMesh.h"
//UserDefinedElem * Pointlocator( Point & point)
//{
//	int n_elem = _UserDefinedElem.size();
//
//	for(int i=0; i<n_elem; i++)
//	{
//		return _UserDefinedElem[i];
//	}
//}

UserDefinedMesh::UserDefinedMesh(const MooseMesh &mesh) :
 _mesh(mesh)
{

}
