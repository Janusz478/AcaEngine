#version 450
#define MAX_LIGHTS 8

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;


uniform int numPointsLights;
uniform vec3 pointLightPos[MAX_LIGHTS];
uniform float pointLightIntensity[MAX_LIGHTS];
uniform vec3 pointLightColor[MAX_LIGHTS];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 InvTransp;

out vec2 uv;
out vec3 Normal;
out vec3 Position;
out vec3 pLightPos[MAX_LIGHTS];

void main() {
	gl_Position = projection * view * model * vec4(vertexPosition, 1);
	Position = vec3(view * model * vec4(vertexPosition, 1));
	for (int i = 0; i < numPointsLights; i++) {
		pLightPos[i] = vec3(view * vec4(pointLightPos[i], 1.0));
	}
	Normal = mat3(InvTransp) * vertexNormal;
	uv = vertexUv;
}
