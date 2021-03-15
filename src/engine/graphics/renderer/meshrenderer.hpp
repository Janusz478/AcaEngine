#pragma once

#include "../core/shader.hpp"
#include "../camera.hpp"
#include "glm/glm.hpp"
#include "mesh.hpp"
#include <vector>
#include <engine/graphics/core/geometrybuffer.hpp>

namespace graphics {

	class Mesh;
	class Texture2D;

	class MeshRenderer
	{
	public:
		MeshRenderer();
		~MeshRenderer();
		void draw(const Mesh& _mesh, const Texture2D& _texture, const glm::mat4& _transform);
		void present(const Camera& _camera);
		void clear();
		void set(const char* _vertex, const char* _fragment);

		Program program;
	private:
		struct MeshInstance {
			const std::vector<Mesh::Vertex>& meshVertices;
			const Texture2D& texture;
			glm::mat4 transform;
			glm::mat3 invTraTransform;
		};
		graphics::GeometryBuffer* geometryBuffer;
		std::vector<MeshInstance> instances;
	};
}
