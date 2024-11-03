#include "PCH.h"
#include "MeshNut.h"
namespace SQ {
	void SQ::MeshNut::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		this->mesh = mesh;
	}

	std::shared_ptr<Mesh> MeshNut::GetMesh()
	{
		return mesh;
	}
}