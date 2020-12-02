#version 450

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

uniform mat4 Transform;
uniform mat4 Camera;

out vec2 uv;
out vec3 Normal;
out vec3 Position;

void main() {
	Position = vec3(Transform * vec4(vertexPosition, 1));
	Normal = mat3(transpose(inverse(Transform))) * vertexNormal;

	uv = vertexUv;
	gl_Position = Camera * Transform * vec4(vertexPosition, 1);
}
