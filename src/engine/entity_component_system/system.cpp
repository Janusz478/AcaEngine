#include "system.hpp"


System::System() : 
	registry(),
	meshRenderer(),
	fontRenderer(graphics::FontManager::get("fonts/AnonymousPro.caf")),
	cameraPerspective(graphics::Camera(45.f, 1.f, 150.f)),
	cameraOrthogonal(graphics::Camera(glm::vec2(1.f * graphics::Device::getAspectRatio(), 1.f))) {}

/* ################ Entity-System ################ */
Entity& System::createEntity(Entity& entity) {
	return entity = registry.create();
}

void System::eraseEntity(Entity& _entity) {
	registry.erase(_entity);
}

EntityRef System::getEntityReference(Entity _entity) const {
	return registry.getRef(_entity);
}

std::optional<Entity> System::getEntity(EntityRef _entity) const {
	return registry.getEntity(_entity);
}


/* ################ Draw-System ################ */
void System::draw() {
	updateAABB();

	// Text draw
	if (!fontRenderer->isEmpty()) fontRenderer->clearText(); 
	registry.execute<Text>([&](Text _text) {	
			fontRenderer->draw(_text.position, _text.text, _text.size, _text.color, _text.rotation, _text.alignX, _text.alignY, _text.roundToPixel);
		});	
	fontRenderer->present(cameraOrthogonal);
		
	// Orthogonal draw	
	meshRenderer.set("shader/mesh3.vert", "shader/mesh3.frag");
	meshRenderer.clear();
	registry.execute<Mesh, Texture, Transform, Orthogonal>([&](
		const Mesh& mesh, const Texture texture, const Transform& transform, const Orthogonal& orthogonal) {
			meshRenderer.draw(*mesh.mesh, *texture.texture, transform.transform);
		});
	meshRenderer.present(cameraOrthogonal);
		
	// Perspective draw
	meshRenderer.set("shader/mesh2.vert", "shader/mesh2.frag");
	registry.execute<Entity, Mesh, Texture, Transform, Perspective>([&](
		const Entity ent, const Mesh& mesh, const Texture texture, const Transform& transform, const Perspective& perspective) {
			meshRenderer.clear();
			meshRenderer.draw(*mesh.mesh, *texture.texture, transform.transform);
			uploadLights(ent);
			meshRenderer.present(cameraPerspective);
		});

}

void System::drawEntity(Entity& _entity, const graphics::Texture2D& _texture) {
	meshRenderer.clear();
	meshRenderer.draw(*registry.getComponent<Mesh>(_entity)->mesh, _texture, registry.getComponent<Transform>(_entity)->transform);
	meshRenderer.present(cameraPerspective);
}

void System::setCameraPerspective(float _fov, float _zNear, float zFar) {
	cameraPerspective = graphics::Camera(_fov, _zNear, zFar);
}

void System::uploadLights(Entity _ent) {
	meshRenderer.program.use();
	//get Constants and upload them
	float constantC;
	float constantQ;
	float constantE;
	registry.execute<LightConstants>([&](LightConstants& lc) {
		constantC = lc.constantC;
		constantQ = lc.constantQ;
		constantE = lc.constantE;
		});
	graphics::glCall(glUniform1f, meshRenderer.program.getUniformLoc("kc"), constantC);
	graphics::glCall(glUniform1f, meshRenderer.program.getUniformLoc("kq"), constantQ);
	graphics::glCall(glUniform1f, meshRenderer.program.getUniformLoc("ke"), constantE);
	
	//find nearest lights and upload them
	Box& box = registry.getComponentUnsafe<Box>(_ent);
	glm::vec3 min = box.transformedAabb.min;
	glm::vec3 max = box.transformedAabb.max;
	float distance;
	float intensity;
	std::map<float, Entity, std::greater<float>> lights;
	registry.execute<Entity, PointLight>([&](Entity ent, PointLight& pl) {
		distance = std::min(glm::distance(min, pl.position), glm::distance(max, pl.position));
		if (distance < pl.areaOfEffect) {
			intensity = pl.intensity / (1 + constantC * distance + constantC * distance * distance + glm::exp(-constantE * distance));
			lights[intensity] = ent;
		}
		});
	int numLights = lights.size();
	const int maxLights = 8;
	if (numLights > maxLights) numLights = maxLights;
	float lightIntensity[maxLights];
	float lightColors[maxLights * 3];
	float lightPositions[maxLights * 3];
	int i = 0;
	for (auto it = lights.begin(); it != lights.end() && i < numLights; it++) {
		PointLight& pl = registry.getComponentUnsafe<PointLight>(it->second);
		lightIntensity[i] = pl.intensity;
		lightColors[i * 3] = pl.color.x;
		lightColors[i * 3 + 1] = pl.color.y;
		lightColors[i * 3 + 2] = pl.color.z;
		lightPositions[i * 3] = pl.position.x;
		lightPositions[i * 3 + 1] = pl.position.y;
		lightPositions[i * 3 + 2] = pl.position.z;
		i++;
	}
	graphics::glCall(glUniform1i, meshRenderer.program.getUniformLoc("numPointsLights"), numLights);
	graphics::glCall(glUniform1fv, meshRenderer.program.getUniformLoc("pointLightIntensity"), numLights, lightIntensity);
	graphics::glCall(glUniform3fv, meshRenderer.program.getUniformLoc("pointLightColor"), numLights, lightColors);
	graphics::glCall(glUniform3fv, meshRenderer.program.getUniformLoc("pointLightPos"), numLights, lightPositions);
}

void System::drawText(std::string _text, glm::vec3 _position, float _size, glm::vec4 _color, float _rotation, float _alignX, float _alignY, bool _roundToPixel) {
	fontRenderer->clearText();
	fontRenderer->draw(_position, _text.c_str(), _size, _color, _rotation, _alignX, _alignY, _roundToPixel);
	fontRenderer->present(cameraOrthogonal);
}


/* ################ Physic-System ################ */
void System::updateTransform(float _deltaTime) {

	registry.execute<Transform, Position>([&](Transform& _transform, const Position& _position)	{
			_transform.transform = glm::translate(glm::mat4(1.0f), _position.position);	
	});

	registry.execute<Transform, Orientation>([&](Transform& _transform, const Orientation& _orientation){							
		_transform.transform *= glm::toMat4(_orientation.orientation);
	});

	registry.execute<Transform, Scale>([&](Transform& _transform, const Scale& _scale){				
		_transform.transform *= glm::scale(glm::mat4(1.0f), _scale.scale);
	});
}

void System::updatePosition(float _deltaTime) {

	registry.execute<Position, Velocity>([&](Position& _position, const Velocity& _velocity)
		{
			_position.position += _velocity.velocity *_deltaTime;
		}
	);
}

void System::updateVelocity(float _deltaTime) {

	registry.execute<Velocity, Accelaration>([&](Velocity& _velocity, const Accelaration& _accelaration)
		{
			_velocity.velocity += _accelaration.accelaration * _deltaTime;
		}
	);
}

void System::updateOrientation(float _deltaTime) {

	registry.execute<Orientation, AngularVelocity>([&](Orientation& _orientation, const AngularVelocity& _angularVelocity)
		{
			_orientation.orientation *= glm::quat(_angularVelocity.angularVelocity * glm::pi<float>() * _deltaTime);
		}
	);
}

void System::updateAABB() {
	registry.execute<Box, Transform>([&](Box& box, Transform transform) {
		std::vector<glm::vec3> corners;
		corners.push_back(glm::vec3(box.aabb.min.x, box.aabb.min.y, box.aabb.min.z));
		corners.push_back(glm::vec3(box.aabb.min.x, box.aabb.min.y, box.aabb.max.z));
		corners.push_back(glm::vec3(box.aabb.min.x, box.aabb.max.y, box.aabb.min.z));
		corners.push_back(glm::vec3(box.aabb.min.x, box.aabb.max.y, box.aabb.max.z));
		corners.push_back(glm::vec3(box.aabb.max.x, box.aabb.min.y, box.aabb.min.z));
		corners.push_back(glm::vec3(box.aabb.max.x, box.aabb.min.y, box.aabb.max.z));
		corners.push_back(glm::vec3(box.aabb.max.x, box.aabb.max.y, box.aabb.min.z));
		corners.push_back(glm::vec3(box.aabb.max.x, box.aabb.max.y, box.aabb.max.z));
		glm::vec3 current;
		auto it = corners.begin();
		glm::vec3 min(glm::vec3(cameraPerspective.getViewProjection() * transform.transform * glm::vec4(*it, 1)));
		glm::vec3 max(glm::vec3(cameraPerspective.getViewProjection() * transform.transform * glm::vec4(*it, 1)));
		it++;
		for (; it != corners.end(); it++) {
			current = glm::vec3(cameraPerspective.getViewProjection() * transform.transform * glm::vec4(*it, 1));
			if (current.x < min.x) min.x = current.x;
			if (current.x > max.x) max.x = current.x;
			if (current.y < min.y) min.y = current.y;
			if (current.y > max.y) max.y = current.y;
			if (current.z < min.z) min.z = current.z;
			if (current.z > max.z) max.z = current.z;
		}
		box.transformedAabb.max = max;
		box.transformedAabb.min = min;
		});
}

int System::removeIntersecting() {
	updateAABB();
	utils::SparseOctree<Entity, 3, float> sparseOctree;
	registry.execute<Entity, Box>([&](Entity ent, Box box) {
		if (!box.isProjectile) sparseOctree.insert(box.transformedAabb, ent);
		});
	std::set<Entity> ent;
	registry.execute<Box>([&](Box box) {
		if (box.isProjectile) {
			utils::SparseOctree<Entity, 3, float>::AABBQuery aabbQuery(box.transformedAabb);
			sparseOctree.traverse(aabbQuery);
			for (auto it = aabbQuery.hits.begin(); it != aabbQuery.hits.end(); it++) {
				ent.insert(*it);
			}
		}
		});
	for (auto it = ent.begin(); it != ent.end(); it++) {
		registry.erase(*it);
	}
	return ent.size();
}

void System::move(Entity& _entity, float _deltaTime) {
	glm::vec3 velocity = registry.getComponent<Velocity>(_entity)->velocity +
		registry.getComponent<Accelaration>(_entity)->accelaration * _deltaTime;
	setVelocity(_entity, velocity);
	Transform& transform = registry.getComponentUnsafe<Transform>(_entity);
	transform.transform *= glm::translate(velocity);
}

void System::springY(Entity& _entity, float _deltaTime) {
	float springConstant = 1.f;
	float gravity = -1.f;

	glm::vec3& accelaration = registry.getComponent<Accelaration>(_entity)->accelaration;
	glm::vec3 position = registry.getComponent<Position>(_entity)->position;	
	glm::vec3 anchor = registry.getComponent<Anchor>(_entity)->anchor;
	float mass = registry.getComponent<Mass>(_entity)->mass;	

	float springForceY = -springConstant * (position[1] - anchor[1]);
	float forceY = springForceY + mass * gravity;
	accelaration[1] = forceY / mass;
}


/* ################ Component-System ################ */
void System::addMesh(Entity& _entity, const graphics::Mesh* _mesh) {
	registry.addComponent<Mesh>(_entity, _mesh);
}

void System::addTransform(Entity& _entity, glm::mat4 _transfrom) {
	registry.addComponent<Transform>(_entity, _transfrom);
}

void System::setTransform(Entity& _entity, glm::mat4 _transform) {
	Transform& transform = registry.getComponentUnsafe<Transform>(_entity);
	transform.transform = _transform;
}

void System::addTexture(Entity& _entity, const graphics::Texture2D* _texture) {
	registry.addComponent<Texture>(_entity, _texture);
}

void System::setTexture(Entity& _entity, const graphics::Texture2D* _texture) {
	auto& texture = registry.getComponentUnsafe<Texture>(_entity);
	texture.texture = _texture;
}

void System::addPosition(Entity& _entity, glm::vec3 _position) {
	registry.addComponent<Position>(_entity, _position);
}

void System::setPosition(Entity& _entity, glm::vec3 _position) {
	Position& position = registry.getComponentUnsafe<Position>(_entity);
	position.position = _position;
}

void System::addOrientation(Entity& _entity, glm::quat _orientation) {
	registry.addComponent<Orientation>(_entity, _orientation);
}

void System::setOrientation(Entity& _entity, glm::quat _orientation) {
	Orientation& orientation = registry.getComponentUnsafe<Orientation>(_entity);
	orientation.orientation = _orientation;
}

void System::addScale(Entity& _entity, glm::vec3 _scale) {
	registry.addComponent<Scale>(_entity, _scale);
}

void System::setScale(Entity& _entity, glm::vec3 _scale) {
	Scale& scale = registry.getComponentUnsafe<Scale>(_entity);
	scale.scale = _scale;
}

void System::addVelocity(Entity& _entity, glm::vec3 _velocity) {
	registry.addComponent<Velocity>(_entity, _velocity);
}

void System::setVelocity(Entity& _entity, glm::vec3 _velocity) {
	Velocity& velocity = registry.getComponentUnsafe<Velocity>(_entity);
	velocity.velocity = _velocity;
}

void System::addAngularVelocity(Entity& _entity, glm::vec3 _angularVelocity) {
	registry.addComponent<AngularVelocity>(_entity, _angularVelocity);
}

void System::setAngularVelocity(Entity& _entity, glm::vec3 _angularVelocity) {
	AngularVelocity& angularVelocity = registry.getComponentUnsafe<AngularVelocity>(_entity);
	angularVelocity.angularVelocity = _angularVelocity;
}

void System::addAccelaration(Entity& _entity, glm::vec3 _velocity) {
	registry.addComponent<Accelaration>(_entity, _velocity);
}

void System::setAccelaration(Entity& _entity, glm::vec3 _accelaration) {
	Accelaration& accelaration = registry.getComponentUnsafe<Accelaration>(_entity);
	accelaration.accelaration = _accelaration;
}

void System::addMass(Entity& _entity, float _mass) {
	registry.addComponent<Mass>(_entity, _mass);
}

void System::setMass(Entity& _entity, float _mass) {
	Mass& mass = registry.getComponentUnsafe<Mass>(_entity);
	mass.mass = _mass;
}

void System::addAnchor(Entity& _entity, glm::vec3 _anchor) {
	registry.addComponent<Anchor>(_entity, _anchor);
}

void System::setAnchor(Entity& _entity, glm::vec3 _anchor) {
	Anchor& anchor = registry.getComponentUnsafe<Anchor>(_entity);
	anchor.anchor = _anchor;
}

void System::addAABB(Entity& ent, bool isProjectile) {
	const graphics::Mesh mesh = *(registry.getComponent<Mesh>(ent)->mesh);
	glm::mat4 transform = cameraPerspective.getViewProjection() * registry.getComponent<Transform>(ent)->transform;
	auto it = mesh.vertices.begin();
	glm::vec3 min(it->Position);
	glm::vec3 max(it->Position);
	glm::vec3 minTransformed(glm::vec3(transform * glm::vec4(it->Position, 1)));
	glm::vec3 maxTransformed(glm::vec3(transform * glm::vec4(it->Position, 1)));
	it++;
	glm::vec3 current;
	glm::vec3 currentTransformed;
	for (; it != mesh.vertices.end(); it++) {
		currentTransformed = glm::vec3(transform * glm::vec4(it->Position, 1));
		if (currentTransformed.x < minTransformed.x) minTransformed.x = currentTransformed.x;
		if (currentTransformed.x > maxTransformed.x) maxTransformed.x = currentTransformed.x;
		if (currentTransformed.y < minTransformed.y) minTransformed.y = currentTransformed.y;
		if (currentTransformed.y > maxTransformed.y) maxTransformed.y = currentTransformed.y;
		if (currentTransformed.z < minTransformed.z) minTransformed.z = currentTransformed.z;
		if (currentTransformed.z > maxTransformed.z) maxTransformed.z = currentTransformed.z;
		current = it->Position;
		if (current.x < min.x) min.x = current.x;
		if (current.x > max.x) max.x = current.x;
		if (current.y < min.y) min.y = current.y;
		if (current.y > max.y) max.y = current.y;
		if (current.z < min.z) min.z = current.z;
		if (current.z > max.z) max.z = current.z;
	}
	registry.addComponent<Box>(ent, isProjectile, math::AABB<3>(min, max), math::AABB<3>(minTransformed, maxTransformed));
}

void System::addPointLight(Entity& _entity, glm::vec3 _position, glm::vec3 _color, float _intensity) {
	float constantC;
	float constantQ;
	float constantE;
	registry.execute<LightConstants>([&](LightConstants& lightConstants) {
		constantC = lightConstants.constantC;
		constantQ = lightConstants.constantQ;
		constantE = lightConstants.constantE;
		});
	float intensity = 0.0f;
	int distance = 0;
	for (; distance < 1000; distance++) {
		intensity = _intensity / (1 + constantC * distance + constantQ * distance * distance + glm::exp(-constantE * distance));
		if (intensity < 0.01) {
			distance = distance - 1;
			break;
		}
	}
	registry.addComponent<PointLight>(_entity, _position, _color, _intensity, distance);
}

void System::addLightConstants(float _constantC, float _constantQ, float _constantE) {
	registry.addComponent<LightConstants>(registry.create(), _constantC, _constantQ, _constantE);
}

void System::setLightConstants(float _constantC, float _constantQ, float _constantE)
{
	registry.execute<LightConstants>([&](LightConstants& lightConstants) {
		lightConstants.constantC = _constantC;
		lightConstants.constantQ = _constantQ;
		lightConstants.constantE = _constantE;
		});
}

void System::addText(Entity& _entity, char* _text, glm::vec3 _position, float _size, glm::vec4 _color, float _rotation, float _alignX, float _alignY, bool _roundToPixel) {
	registry.addComponent<Text>(_entity, _text, _position, _size, _color, _rotation, _alignX, _alignY, _roundToPixel);
}

void System::setText(Entity& _entity, char* _text, glm::vec3 _position, float _size, glm::vec4 _color, float _rotation, float _alignX, float _alignY, bool _roundToPixel) {
	Text& text = registry.getComponentUnsafe<Text>(_entity);
	text.text = _text;
	text.position = _position;
	text.size = _size;
	text.color = _color;
	text.rotation = _rotation;
	text.alignX = _alignX;
	text.alignY = _alignY;
	text.roundToPixel = _roundToPixel;
}

void System::addOrthogonal(Entity& _entity) {
	registry.addComponent<Orthogonal>(_entity);
}

void System::addPerspective(Entity& _entity) {
	registry.addComponent<Perspective>(_entity);
}

void System::addAlternativeTexture(Entity& _entity, const graphics::Texture2D* _texture, bool _inUse) {
	registry.addComponent<AlternativeTexture>(_entity, _texture, _inUse);
}

void System::addButton(Entity& _entity, bool _swapTextureIfCursorOnIt, bool _activeFunction) {
	registry.addComponent<Button>(_entity, _swapTextureIfCursorOnIt, _activeFunction);
}

void System::addTextDisplay(Entity& _entity) {
	registry.addComponent<TextDisplay>(_entity);
}

void System::addTextField(Entity& _entity, bool _pressed) {
	registry.addComponent<TextField>(_entity, _pressed);
}

void System::addCheckBox(Entity& _entity, uint32_t _buttonEntity, uint32_t _textEntity, bool _pressed, bool _status) {
	registry.addComponent<CheckBox>(_entity, _buttonEntity, _textEntity, _pressed, _status);
}

void System::addContainer(Entity& _entity, int _rows, int _colums, float _gapFactor, bool _pressed, glm::vec2 _oldCursorPosition, float _time) {
	registry.addComponent<Container>(_entity, _rows, _colums, _gapFactor, _pressed, _oldCursorPosition, _time);
}

void System::addFunction(Entity& _entity, void (*_function)(Entity& _entity, System& _system)) {
	registry.addComponent<Function>(_entity, _function);
}

void System::addMoved(Entity& _entity, bool _moved) {
	registry.addComponent<Moved>(_entity, _moved);
}

void System::addMovable(Entity& _entity, bool _movable) {
	registry.addComponent<Movable>(_entity, _movable);
}

void System::addSlider(Entity& _entity, Slider _slider){
	registry.addComponent<Slider>(_entity, _slider);
}

void System::addBox2D(Entity& _entity, glm::vec2 _p1, glm::vec2 _p2) {
	registry.addComponent<Box2D>(_entity, math::Rectangle(_p1, _p2));
}


/* ################ Utils-System ################ */
int System::randomWithoutZero(int quantity, int start) {
	int random = 0;
	while (random == 0) {
		random = rand() % quantity + (start);
	};
	return random;
}

int System::whichEntityIsNotInView() {
	int found = -1;
	registry.execute<Entity, Transform>([&found](Entity _entity, const Transform& transform) {
		glm::vec3 position = glm::vec3(transform.transform[3][0], transform.transform[3][1], transform.transform[3][2]);
		if (position[0] < (-76.f) || position[0] > (76.f) ||
			position[1] < (-51.f) || position[1] > (51.f) ||
			position[2] < (-150.f) || position[2] > (0.f))
		{
			found = _entity.id;
			return found;
		}});
	return found;
}
