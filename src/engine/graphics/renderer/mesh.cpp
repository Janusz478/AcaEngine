#include "mesh.hpp"


namespace graphics {

	Mesh::Mesh(const utils::MeshData& _meshData) {
		for (auto it = _meshData.faces.begin(); it != _meshData.faces.end(); ++it) {
			Vertex v0 = {
				_meshData.positions[it->indices[0].positionIdx],
				_meshData.normals[it->indices[0].normalIdx.value()],
				_meshData.textureCoordinates[it->indices[0].textureCoordinateIdx.value()]
			};
			Vertex v1 = {
				_meshData.positions[it->indices[1].positionIdx],
				_meshData.normals[it->indices[1].normalIdx.value()],
				_meshData.textureCoordinates[it->indices[1].textureCoordinateIdx.value()]
			};
			Vertex v2 = {
				_meshData.positions[it->indices[2].positionIdx],
				_meshData.normals[it->indices[2].normalIdx.value()],
				_meshData.textureCoordinates[it->indices[2].textureCoordinateIdx.value()]
			};
			vertices.push_back(v0);
			vertices.push_back(v1);
			vertices.push_back(v2);
		}
	}

}