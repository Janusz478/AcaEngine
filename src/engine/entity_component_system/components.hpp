#pragma once

#include "../graphics/renderer/mesh.hpp"
#include "../graphics/core/texture.hpp"
#include "../math/geometrictypes.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <functional>
#include "registry.hpp"


struct Mesh {
	const graphics::Mesh* mesh;
};

struct Transform {
	glm::mat4 transform;
};

struct Texture {
	const graphics::Texture2D* texture;
};

struct Position {
	glm::vec3 position;
};

struct Orientation {
	glm::quat orientation;
};

struct Scale {
	glm::vec3 scale;
};

struct Velocity {
	glm::vec3 velocity;
};

struct AngularVelocity {
	glm::vec3 angularVelocity;
};

struct Accelaration {
	glm::vec3 accelaration;
};

struct Mass {
	float mass;
};

struct Anchor {
	glm::vec3 anchor;
};

struct Box {
	bool isProjectile;
	math::AABB<3> aabb;
	math::AABB<3> transformedAabb;
};

struct Box2D {
	math::Rectangle box;
};

struct PointLight {
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
	int areaOfEffect;
};

struct LightConstants {
	float constantC;
	float constantQ;
	float constantE;
};

struct Text {
	char* text;
	glm::vec3 position;
	float size;
	glm::vec4 color;
	float rotation;
	float alignX;
	float alignY;
	bool roundToPixel;
};

struct Orthogonal {};

struct Perspective {};

struct AlternativeTexture {
	const graphics::Texture2D* texture;
	bool inUse;
};

struct Slider {
	Entity backgroundEntity;
	Entity* levelEntities;
	int currentLevel;
	int numberOfLevels;
	bool vertical;
};

struct Button {
	bool swapTextureIfCursorOnIt;
	bool activeFunction;
};

struct TextDisplay {};

struct TextField {
	bool pressed;
};

struct CheckBox {
	uint32_t buttonEntity;
	uint32_t textEntity;
	bool pressed;
	bool status;
};

struct Container{
	int rows;
	int colums;
	float gapFactor;
	bool pressed;
	glm::vec2 oldCursorPosition;
	float time;
};

struct Moved {
	bool moved;
};

struct Movable {
	bool movable;
};
