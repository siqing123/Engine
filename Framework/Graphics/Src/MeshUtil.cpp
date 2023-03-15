#include "Precompiled.h"
#include "MeshUtil.h"

using namespace Storm;
using namespace Storm::Graphics;

void MeshUtil::ComputeNormal(Mesh& mesh)
{
	std::vector<Math::Vector3> accumulateNormals;
	accumulateNormals.resize(mesh.vertices.size());
	for (size_t i = 0; i < mesh.indices.size(); i += 3)
	{
		const auto ia = mesh.indices[i + 0];
		const auto ib = mesh.indices[i + 1];
		const auto ic = mesh.indices[i + 2];

		auto& va = mesh.vertices[ia];
		auto& vb = mesh.vertices[ib];
		auto& vc = mesh.vertices[ic];

		const auto ab = Math::Normalize(vb.position - va.position);
		const auto ac = Math::Normalize(vc.position - va.position);
		const auto normal = Math::Normalize(Math::Cross(ab, ac));

		accumulateNormals[ia] += normal;
		accumulateNormals[ib] += normal;
		accumulateNormals[ic] += normal;
	}
	for (size_t i = 0; i < mesh.vertices.size(); ++i)
		mesh.vertices[i].normal = Math::Normalize(accumulateNormals[i]);
}