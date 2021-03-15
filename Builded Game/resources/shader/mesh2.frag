#version 450
#define MAX_LIGHTS 8

in vec2 uv;
in vec3 Normal;
in vec3 Position;
in vec3 pLightPos[MAX_LIGHTS];

out vec3 color;

uniform float pointLightIntensity[MAX_LIGHTS];
uniform vec3 pointLightColor[MAX_LIGHTS];
uniform int numPointsLights;

uniform float kc;
uniform float kq;
uniform float ke;
uniform sampler2D myTexture;


void main() {
    //ambient
    float ambientStrength = 0.2;
    vec3 ambient = vec3(ambientStrength);

    vec3 sum = ambient;
    for(int i = 0; i < numPointsLights; i++) {
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(pLightPos[i] - Position);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = vec3(diff);
        //Specular
        float specularStrength = 2.0;
        vec3 viewDir = normalize(-Position);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
        vec3 specular = vec3(specularStrength * spec);
        //Intensity
        float distance = distance(pLightPos[i], Position);
        float intensity = pointLightIntensity[i] / (1 + kc * distance + kq * distance + exp(ke * distance));

        sum = sum + intensity * (diffuse + specular) * pointLightColor[i];
    }
    vec3 tex = texture(myTexture, uv).rgb;
    color = tex * sum;
}