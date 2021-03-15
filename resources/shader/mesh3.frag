#version 450

in vec2 uv;

out vec3 color;

uniform sampler2D myTexture;

void main() {
    color = texture(myTexture, uv).rgb;
}
