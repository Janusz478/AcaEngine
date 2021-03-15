#pragma once
#include "../entity_component_system/components.hpp"
#include <engine/entity_component_system/system.hpp>

class EntityCreationInterface
{
public:

	static void initialize();

	static Entity createMovingPlanet(System& _system, glm::mat4 _transform, glm::vec3 _velocity, glm::vec3 _accelaration);

	static Entity createMovingCrate(System& _system, glm::mat4 _transform, glm::vec3 _velocity, glm::vec3 _accelaration);

	static Entity createMovingObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform,
		glm::vec3 _velocity, glm::vec3 _accelaration);


	static Entity createSpringPlanet(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _accelaration,
		float _mass, glm::vec3 _anchor);

	static Entity createSpringCrate(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _accelaration,
		float _mass, glm::vec3 _anchor);

	static Entity createSpringObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform,
		glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _accelaration, float _mass, glm::vec3 _anchor);


	static Entity createRotatingPlanet(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation,
		glm::vec3 _velocity, glm::vec3 _angularVelocity, bool _isProjectile);

	static Entity createRotatingCrate(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation,
		glm::vec3 _velocity, glm::vec3 _angularVelocity, bool _isProjectile);

	static Entity createRotatingObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform,
		glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, glm::vec3 _velocity, glm::vec3 _angularVelocity, bool _isProjectile);


	static Entity createShootablePlanet(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::vec3 _velocity,
		bool _isProjectile);

	static Entity createShootableCrate(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::vec3 _velocity,
		bool _isProjectile);

	static Entity createShootableObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform,
		glm::vec3 _position, glm::vec3 _scale, glm::vec3 _velocity, bool _isProjectile);


	static Entity createPlanet(System& _system, glm::mat4 _transform, bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale,
		bool _hasOrientation, glm::quat _orientation, bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity,
		bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor,
		bool _hasBox, bool _isProjectile);

	static Entity createCrate(System& _system, glm::mat4 _transform, bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale,
		bool _hasOrientation, glm::quat _orientation, bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity,
		bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor,
		bool _hasBox, bool _isProjectile);

	static Entity createWorldObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform,
		bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale, bool _hasOrientation, glm::quat _orientation,
		bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity, bool _hasAccelaration,
		glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor, bool _hasBox, bool _isProjectile);


	static Entity createRectangle(System& _system, const graphics::Texture2D* _texture, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale);

	//For the Point Lights to work, it is necessary, that the objects have an AABB
	static std::vector<Entity> createPointLights(System& _system, float c_constant, float q_constant, float e_constant, std::vector<glm::vec3> positions,
		std::vector<glm::vec3> colors, std::vector<float> intensities);


private:
	static const graphics::Texture2D* texturePlanet;
	static const graphics::Texture2D* textureCratetex;
	static const graphics::Mesh& meshSphere;
	static const graphics::Mesh& meshCrate;
	static const graphics::Mesh& meshRectangle;
};