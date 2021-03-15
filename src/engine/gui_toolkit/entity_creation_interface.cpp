#include "entity_creation_interface.hpp"
#include "entity_creation_interface.hpp"
#include "entity_creation_interface.hpp"
#include "entity_creation_interface.hpp"
#include "entity_creation_interface.hpp"

const graphics::Texture2D* EntityCreationInterface::texturePlanet = nullptr;
const graphics::Texture2D* EntityCreationInterface::textureCratetex = nullptr;
const graphics::Mesh& EntityCreationInterface::meshSphere = *utils::MeshLoader::get("models/sphere.obj");
const graphics::Mesh& EntityCreationInterface::meshCrate = *utils::MeshLoader::get("models/crate.obj");
const graphics::Mesh& EntityCreationInterface::meshRectangle = *utils::MeshLoader::get("models/plane.obj");


void EntityCreationInterface::initialize() {
	if (texturePlanet == nullptr)
		texturePlanet = graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR));
	if (textureCratetex == nullptr)
		textureCratetex = graphics::Texture2DManager::get("textures/cratetex.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR));
}

Entity EntityCreationInterface::createMovingPlanet(System& _system, glm::mat4 _transform, glm::vec3 _velocity, glm::vec3 _accelaration) {
	return createMovingObject(_system, &meshSphere, texturePlanet, _transform, _velocity, _accelaration);
}

Entity EntityCreationInterface::createMovingCrate(System& _system, glm::mat4 _transform, glm::vec3 _velocity, glm::vec3 _accelaration) {
	return createMovingObject(_system, &meshCrate, textureCratetex, _transform, _velocity, _accelaration);
}

Entity EntityCreationInterface::createMovingObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture,
	glm::mat4 _transform, glm::vec3 _velocity, glm::vec3 _accelaration) {
	Entity entity;
	_system.createEntity(entity);
	_system.addMesh(entity, _mesh);
	_system.addTexture(entity, _texture);
	_system.addTransform(entity, _transform);
	_system.addVelocity(entity, _velocity);
	_system.addAccelaration(entity, _accelaration);
	_system.addPerspective(entity);
	_system.addAABB(entity, true);
	return entity;
}


Entity EntityCreationInterface::createSpringPlanet(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _velocity,
	glm::vec3 _accelaration, float _mass, glm::vec3 _anchor) {
	return createSpringObject(_system, &meshSphere, texturePlanet, _transform, _position, _velocity, _accelaration, _mass, _anchor);
}

Entity EntityCreationInterface::createSpringCrate(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _velocity,
	glm::vec3 _accelaration, float _mass, glm::vec3 _anchor) {
	return createSpringObject(_system, &meshCrate, textureCratetex, _transform, _position, _velocity, _accelaration, _mass, _anchor);
}

Entity EntityCreationInterface::createSpringObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture,
	glm::mat4 _transform, glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _accelaration, float _mass, glm::vec3 _anchor) {
	Entity entity;
	_system.createEntity(entity);
	_system.addMesh(entity, _mesh);
	_system.addTexture(entity, _texture);
	_system.addTransform(entity, _transform);
	_system.addPosition(entity, _position);
	_system.addVelocity(entity, _velocity);
	_system.addAccelaration(entity, _accelaration);
	_system.addMass(entity, _mass);
	_system.addAnchor(entity, _anchor);
	_system.addPerspective(entity);
	_system.addAABB(entity, true);
	return entity;
}


Entity EntityCreationInterface::createRotatingPlanet(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation,
	glm::vec3 _velocity, glm::vec3 _angularVelocity, bool _isProjectile) {
	return createRotatingObject(_system, &meshSphere, texturePlanet, _transform, _position, _scale, _orientation, _velocity, _angularVelocity,
		_isProjectile);
}

Entity EntityCreationInterface::createRotatingCrate(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation,
	glm::vec3 _velocity, glm::vec3 _angularVelocity, bool _isProjectile) {
	return createRotatingObject(_system, &meshCrate, textureCratetex, _transform, _position, _scale, _orientation, _velocity, _angularVelocity,
		_isProjectile);
}

Entity EntityCreationInterface::createRotatingObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture,
	glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, glm::vec3 _velocity, glm::vec3 _angularVelocity,
	bool _isProjectile) {
	Entity entity;
	_system.createEntity(entity);
	_system.addMesh(entity, _mesh);
	_system.addTexture(entity, _texture);
	_system.addTransform(entity, _transform);
	_system.addPosition(entity, _position);
	_system.addScale(entity, _scale);
	_system.addOrientation(entity, _orientation);
	_system.addVelocity(entity, _velocity);
	_system.addAngularVelocity(entity, _angularVelocity);
	_system.addPerspective(entity);
	_system.addAABB(entity, _isProjectile);
	return entity;
}


Entity EntityCreationInterface::createShootablePlanet(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale,
	glm::vec3 _velocity, bool _isProjectile) {
	return createShootableObject(_system, &meshSphere, texturePlanet, _transform, _position, _scale, _velocity, _isProjectile);
}

Entity EntityCreationInterface::createShootableCrate(System& _system, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale,
	glm::vec3 _velocity, bool _isProjectile) {
	return createShootableObject(_system, &meshCrate, textureCratetex, _transform, _position, _scale, _velocity, _isProjectile);
}

Entity EntityCreationInterface::createShootableObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture,
	glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::vec3 _velocity, bool _isProjectile) {
	Entity entity;
	_system.createEntity(entity);
	_system.addMesh(entity, _mesh);
	_system.addTexture(entity, _texture);
	_system.addTransform(entity, _transform);
	_system.addPosition(entity, _position);
	_system.addScale(entity, _scale);
	_system.addVelocity(entity, _velocity);
	_system.addPerspective(entity);
	_system.addAABB(entity, _isProjectile);
	return entity;
}


Entity EntityCreationInterface::createPlanet(System& _system, glm::mat4 _transform, bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale,
	bool _hasOrientation, glm::quat _orientation, bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity,
	bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor,
	bool _hasBox, bool _isProjectile) {
	return createWorldObject(_system, &meshSphere, texturePlanet, _transform, _hasPosition, _position, _hasScale, _scale, _hasOrientation, _orientation,
		_hasVelocity, _velocity, _hasAngularVelocity, _angularVelocity, _hasAccelaration, _accelaration, _hasMass, _mass, _hasAnchor, _anchor,
		_hasBox, _isProjectile);
}

Entity EntityCreationInterface::createCrate(System& _system, glm::mat4 _transform, bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale,
	bool _hasOrientation, glm::quat _orientation, bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity,
	bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor,
	bool _hasBox, bool _isProjectile) {
	return createWorldObject(_system, &meshCrate, textureCratetex, _transform, _hasPosition, _position, _hasScale, _scale, _hasOrientation, _orientation,
		_hasVelocity, _velocity, _hasAngularVelocity, _angularVelocity,	_hasAccelaration, _accelaration, _hasMass, _mass, _hasAnchor, _anchor,
		_hasBox, _isProjectile);
}

Entity EntityCreationInterface::createWorldObject(System& _system, const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform,
	bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale, bool _hasOrientation, glm::quat _orientation, bool _hasVelocity,
	glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity, bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass,
	float _mass, bool _hasAnchor, glm::vec3 _anchor, bool _hasBox, bool _isProjectile) {
	Entity entity;
	_system.createEntity(entity);
	_system.addMesh(entity, _mesh);
	_system.addTexture(entity, _texture);
	_system.addTransform(entity, _transform);
	if (_hasPosition) _system.addPosition(entity, _position);
	if (_hasScale) _system.addScale(entity, _scale);
	if (_hasOrientation) _system.addOrientation(entity, _orientation);
	if (_hasVelocity) _system.addVelocity(entity, _velocity);
	if (_hasAngularVelocity) _system.addAngularVelocity(entity, _angularVelocity);
	if (_hasAccelaration) _system.addAccelaration(entity, _accelaration);
	if (_hasMass) _system.addMass(entity, _mass);
	if (_hasAnchor) _system.addAnchor(entity, _anchor);
	if (_hasBox) _system.addAABB(entity, _isProjectile);
	_system.addPerspective(entity);
	return entity;
}

Entity EntityCreationInterface::createRectangle(System& _system, const graphics::Texture2D* _texture, glm::mat4 _transform, glm::vec3 _position,
	glm::vec3 _scale) {
	Entity entity;
	_system.createEntity(entity);
	_system.addMesh(entity, &meshRectangle);
	_system.addTexture(entity, _texture);
	_system.addTransform(entity, _transform);
	_system.addPosition(entity, _position);
	_system.addScale(entity, _scale);
	_system.addOrthogonal(entity);
	return entity;
}

std::vector<Entity> EntityCreationInterface::createPointLights(System& _system, float c_constant, float q_constant, float e_constant,
	std::vector<glm::vec3> positions, std::vector<glm::vec3> colors, std::vector<float> intensities) {
	std::vector<Entity> entities;
	_system.addLightConstants(c_constant, q_constant, e_constant);
	for (int i = 0; i < positions.size(); i++) {
		Entity entity;
		_system.createEntity(entity);
		entities.push_back(entity);
		_system.addPointLight(entity, positions[i], colors[i], intensities[i]);
	}
	return entities;
}