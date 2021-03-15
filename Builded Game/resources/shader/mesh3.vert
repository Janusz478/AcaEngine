#version 450

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 InvTransp;

out vec2 uv;
out vec3 Normal;
out vec3 Position;
out vec3 lightPos;

void main() {
	gl_Position = projection * view * model * vec4(vertexPosition, 1);
	Position = vec3(view * model * vec4(vertexPosition, 1));
	lightPos = vec3(view * vec4(2.0, 1.0, 2.0, 1.0));
	Normal = mat3(InvTransp) * vertexNormal;
	uv = vertexUv;
}
