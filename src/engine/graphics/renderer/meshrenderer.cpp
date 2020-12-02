#include "meshrenderer.hpp"
#include "../core/opengl.hpp"
#include "../core/texture.hpp"
#include "mesh.hpp"
#include <engine/graphics/core/geometrybuffer.hpp>


namespace graphics {

	MeshRenderer::MeshRenderer() {
		program.attach(ShaderManager::get("shader/mesh.vert", ShaderType::VERTEX));
		program.attach(ShaderManager::get("shader/mesh.frag", ShaderType::FRAGMENT));
		program.link();

		graphics::VertexAttribute va[3];
		va[0] = {graphics::PrimitiveFormat::FLOAT, 3, false, false};
		va[1] = {graphics::PrimitiveFormat::FLOAT, 3, false, false};
		va[2] = {graphics::PrimitiveFormat::FLOAT, 2, false, false};
		geometryBuffer = new GeometryBuffer(graphics::GLPrimitiveType::TRIANGLES, va, 3, 0);
	}

	MeshRenderer::~MeshRenderer() {
		delete geometryBuffer;
	}

	void MeshRenderer::draw(const Mesh& _mesh, const Texture2D& _texture, const glm::mat4& _transform) {
		struct MeshInstance mi = {_mesh.vertices, _texture, _transform};
		instances.push_back(mi);
	}

	void MeshRenderer::present(const Camera& _camera) {
		if (instances.empty()) return;
		program.use();
		program.setUniform(program.getUniformLoc("Camera"), _camera.getViewProjection());
		for (auto it = instances.begin(); it != instances.end(); it++) {

			auto p = std::find(textureIds.begin(), textureIds.end(), it->texture.getID());
			if (p != textureIds.end()) {
				it->texture.bind(0);
				textureIds.push_back(it->texture.getID());
			}
			
			program.setUniform(program.getUniformLoc("Transform"),it->transform);
			geometryBuffer->setData(&(it->meshVertices[0]), it->meshVertices.size() * sizeof(Mesh::Vertex));
			geometryBuffer->draw();
		}
		
	}

	void MeshRenderer::clear() {
		instances.clear();
	}

}
