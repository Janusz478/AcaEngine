#include "meshrenderer.hpp"
#include "../core/opengl.hpp"
#include "../core/texture.hpp"
#include "mesh.hpp"
#include <engine/graphics/core/geometrybuffer.hpp>


namespace graphics {

	MeshRenderer::MeshRenderer() {
		program.attach(ShaderManager::get("shader/mesh2.vert", ShaderType::VERTEX));
		program.attach(ShaderManager::get("shader/mesh2.frag", ShaderType::FRAGMENT));
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
		struct MeshInstance mi = {_mesh.vertices, _texture, _transform, glm::mat3(glm::transpose(glm::inverse(_transform)))};
		instances.push_back(mi);
	}

	void MeshRenderer::present(const Camera& _camera) {
		if (instances.empty()) return;
		program.use();
		program.setUniform(program.getUniformLoc("view"), _camera.getView());
		program.setUniform(program.getUniformLoc("projection"), _camera.getProjection());
		for (auto it = instances.begin(); it != instances.end(); it++) {
			it->texture.bind(0);
			program.setUniform(program.getUniformLoc("model"), it->transform);
			program.setUniform(program.getUniformLoc("InvTransp"), glm::transpose(glm::inverse(_camera.getView() * it->transform)));
			geometryBuffer->setData(&(it->meshVertices[0]), it->meshVertices.size() * sizeof(Mesh::Vertex));
			geometryBuffer->draw();
		}
		
	}

	void MeshRenderer::clear() {
		instances.clear();
	}
	
	void MeshRenderer::set(const char* _vertex, const char* _fragment) 
	{
		program.clear();
		program.attach(ShaderManager::get(_vertex, ShaderType::VERTEX));
		program.attach(ShaderManager::get(_fragment, ShaderType::FRAGMENT));
		program.link();
	}

}



